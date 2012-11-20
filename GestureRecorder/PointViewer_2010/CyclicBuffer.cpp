
#include <XnOpenNI.h>
#include <XnLog.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>
#include "../../GestureDetection4D/Datasource.h"
#include <vector>
using namespace xn;

#define CHECK_RC(rc, what)											\
  if (rc != XN_STATUS_OK)											\
{																\
  printf("%s failed: %s\n", what, xnGetStatusString(rc));		\
  return rc;													\
}

#define CHECK_RC_ERR(rc, what, error)			\
{												\
  if (rc == XN_STATUS_NO_NODE_PRESENT)		\
{											\
  XnChar strError[1024];					\
  errors.ToString(strError, 1024);		\
  printf("%s\n", strError);				\
}											\
  CHECK_RC(rc, what)							\
}

extern std::vector<XnPoint3D> g_handPoints;
struct RecConfiguration
{
  RecConfiguration()
  {
    pDepthMode = &XnMapOutputMode();
    pImageMode = &XnMapOutputMode();
    bRecordDepth = TRUE;
    bRecordImage = TRUE;

    bMirrorIndicated = FALSE;
    bMirror = TRUE;

    bRegister = FALSE;
    bFrameSync = FALSE;
    bVerbose = FALSE;

    nDumpTime = 1;
    sprintf(strDirName, LOCAL_SAVEDIR);
  }
  XnMapOutputMode* pDepthMode;
  XnMapOutputMode* pImageMode;
  XnBool bRecordDepth;
  XnBool bRecordImage;


  XnBool bMirrorIndicated;
  XnBool bMirror;
  XnBool bRegister;
  XnBool bFrameSync;
  XnBool bVerbose;

  XnUInt32 nDumpTime;
  XnChar strDirName[XN_FILE_MAX_PATH];
};

// The cyclic buffer, to which frames will be added and from where they will be dumped to files
class CyclicBuffer
{

public:
  // Creation - set the OpenNI objects
  CyclicBuffer(xn::Context& context, xn::DepthGenerator& depthGenerator, xn::ImageGenerator& imageGenerator, const RecConfiguration& config) :
      m_context(context),
        m_depthGenerator(depthGenerator),
        m_imageGenerator(imageGenerator)
      {
        m_bDepth = config.bRecordDepth;
        m_bImage = config.bRecordImage;
        m_nNextWrite = 0;
        m_nBufferSize = 0;
        m_nBufferCount = 0;
        this->datasource=new Datasource();
      }

      // Initialization - set outdir and time of each recording
      void Initialize(XnChar* strDirName, XnUInt32 nSeconds)
      {
        xnOSStrCopy(m_strDirName, strDirName, XN_FILE_MAX_PATH);
		m_nBufferSize = nSeconds*30;
        m_pFrames = XN_NEW_ARR(SingleFrame, m_nBufferSize);
      }

      // Save new data from OpenNI
      void Update(const xn::DepthGenerator& depthGenerator, const xn::ImageGenerator& imageGenerator)
      {
        if (m_bDepth)
        {
          // Save latest depth frame
          xn::DepthMetaData dmd;
          depthGenerator.GetMetaData(dmd);
          m_pFrames[m_nNextWrite].depthFrame.CopyFrom(dmd);
        }
        if (m_bImage)
        {
          // Save latest image frame
          xn::ImageMetaData imd;
          imageGenerator.GetMetaData(imd);
          m_pFrames[m_nNextWrite].imageFrame.CopyFrom(imd);
        }
        // See if buffer is already full
        if (m_nBufferCount < m_nBufferSize)
        {
          m_nBufferCount++;
        }
        // Make sure cylic buffer pointers are good
        m_nNextWrite++;
        if (m_nNextWrite == m_nBufferSize)
        {
          m_nNextWrite = 0;
        }
      }

	  void reset(){
		m_nNextWrite = 0;
        m_nBufferCount = 0;
		delete[] m_pFrames;
		m_pFrames = XN_NEW_ARR(SingleFrame, m_nBufferSize);
	  }

      // Save the current state of the buffer to a file
      XnStatus Dump()
      {
        xn::MockDepthGenerator mockDepth;
        xn::MockImageGenerator mockImage;

        xn::EnumerationErrors errors;
        XnStatus rc;

        // Create recorder
        rc = m_context.CreateAnyProductionTree(XN_NODE_TYPE_RECORDER, NULL, m_recorder, &errors);
        CHECK_RC_ERR(rc, "Create recorder", errors);

        // Create name of new file
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        // TODO: Remove hardcoded directory
		sprintf(strFileName, "%s/%04d%02d%02d-%02d%02d%02d.oni",  LOCAL_SAVEDIR,
          timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

        m_recorder.SetDestination(XN_RECORD_MEDIUM_FILE, strFileName);
        printf("Creating file %s\n", strFileName);
        strFileName;

        // Create mock nodes based on the depth generator, to save depth
        if (m_bDepth)
        {
          // Create mock nodes based on the depth generator, to save depth
          rc = m_context.CreateMockNodeBasedOn(m_depthGenerator, NULL, mockDepth);
          CHECK_RC(rc, "Create depth node");
          rc = m_recorder.AddNodeToRecording(mockDepth, XN_CODEC_16Z_EMB_TABLES);
          CHECK_RC(rc, "Add depth node");
        }

        if (m_bImage)
        {
          // Create mock nodes based on the image generator, to save image
          rc = m_context.CreateMockNodeBasedOn(m_imageGenerator, NULL, mockImage);
          CHECK_RC(rc, "Create image node");
          rc = m_recorder.AddNodeToRecording(mockImage, XN_CODEC_JPEG);
          CHECK_RC(rc, "Add image node");
        }

        // Write frames from next index (which will be next to be written, and so the first available)
        // this is only if a full loop was done, and this frame has meaningful data
        if (m_nNextWrite < m_nBufferCount)
        {
          // Not first loop, right till end
          for (XnUInt32 i = m_nNextWrite; i < m_nBufferSize; ++i)
          {
            if (m_bDepth)
            {
              mockDepth.SetData(m_pFrames[i].depthFrame);
            }
            if (m_bImage)
            {
              mockImage.SetData(m_pFrames[i].imageFrame);
            }

            m_recorder.Record();
          }
        }
        // Write frames from the beginning of the buffer to the last on written
        for (XnUInt32 i = 0; i < m_nNextWrite; ++i)
        {
          if (m_bDepth)
          {
            mockDepth.SetData(m_pFrames[i].depthFrame);
          }
          if (m_bImage)
          {
            mockImage.SetData(m_pFrames[i].imageFrame);
          }

          m_recorder.Record();
        }

        // Close recorder
        m_recorder.Release();
        mockImage.Release();
        mockDepth.Release();
		reset();
        return XN_STATUS_OK;
      }
	
      // Creates a new DatabaseEntry of the OniFile
      void writeDumpToDB(XnPoint3D* start,char* gesture){
	  this->datasource->createNewFileEntry(this->strFileName, start,gesture,g_handPoints);
	  g_handPoints.clear();
      //############################Testcode to verify the functionality#######################
		//  vector <OniFileDataSet*> oniFileDataSets = this->datasource->getOniFileDatasetsByGesture(gesture);
	  
      }

protected:
  struct SingleFrame
  {
    xn::DepthMetaData depthFrame;
    xn::ImageMetaData imageFrame;
  };

  XnBool m_bDepth, m_bImage;
  SingleFrame* m_pFrames;
  XnUInt32 m_nNextWrite;
  XnUInt32 m_nBufferSize;
  XnUInt32 m_nBufferCount;
  XnChar m_strDirName[XN_FILE_MAX_PATH];

  xn::Context& m_context;
  xn::DepthGenerator& m_depthGenerator;
  xn::ImageGenerator& m_imageGenerator;
  xn::Recorder m_recorder;
  Datasource* datasource;
  XnChar strFileName[XN_FILE_MAX_PATH];

 private:
  	bool expandFrames() {
		XnUInt32 preFrames = 30; // TODO CONSTANT!
		SingleFrame* m_pPreFrames = XN_NEW_ARR(SingleFrame, m_nBufferSize + preFrames);
		
		for (XnUInt32 i = 0; i < preFrames; i++) {
			m_pPreFrames[i].depthFrame.CopyFrom(m_pFrames[preFrames - i].depthFrame);
			m_pPreFrames[i].imageFrame.CopyFrom(m_pFrames[preFrames - i].imageFrame);
		}
		for (XnUInt32 i = 0; i < m_nBufferSize; i++) {
			m_pPreFrames[i + preFrames].depthFrame.CopyFrom(m_pFrames[i].depthFrame);
			m_pPreFrames[i + preFrames].imageFrame.CopyFrom(m_pFrames[i].imageFrame);
		}
		delete[] m_pFrames;
		m_pFrames = m_pPreFrames;
		
		return (m_pFrames != NULL);
	}

};
