using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using agora_gaming_rtc;
using agora_utilities;
using UnityEngine.EventSystems;
public class GameManager : MonoBehaviour
{
    public string AppID;
    public string ChannelName;

    GameObject myFace;
    GameObject remote;
    GameObject faceContent;
    GameObject addButton;
    GameObject test;
    VideoSurface myView;
    VideoSurface remoteView;
    IRtcEngine mRtcEngine;
    private string TOKEN = "SECRET_KEY";
    private const float Offset = 100;
    private int userCount = 0;
    void Awake()
    {
        SetupUI();
        
    }
    void Start()
    {
        setupAgora();

    }
    void SetupUI()
    {
        test = new GameObject("Raw Image");
        test.AddComponent<RawImage>();
        test.AddComponent<Button>();
        //test.rectTransform.sizeDelta = new Vector2(200, test.rectTransform.sizeDelta.y);
        myFace = GameObject.Find("MyView");
        myView = myFace.AddComponent<VideoSurface>();
        remote = GameObject.Find("RemoteView");
        remote?.GetComponent<Button>()?.onClick.AddListener(changeObject);

        setUI(false);
        GameObject go;
        go = GameObject.Find("LeaveButton");
        go?.GetComponent<Button>()?.onClick.AddListener(Leave);

        go = GameObject.Find("JoinButton");
        go?.GetComponent<Button>()?.onClick.AddListener(Join);

        addButton = GameObject.Find("add");
        addButton?.GetComponent<Button>()?.onClick.AddListener(onClickAdd);

    }

    void setupAgora()
    {

        mRtcEngine = IRtcEngine.GetEngine(AppID);

        mRtcEngine.SetLogFile("log.txt");
        mRtcEngine.SetChannelProfile(CHANNEL_PROFILE.CHANNEL_PROFILE_LIVE_BROADCASTING);
        mRtcEngine.SetClientRole(CLIENT_ROLE_TYPE.CLIENT_ROLE_BROADCASTER);
        mRtcEngine.EnableAudio();
        mRtcEngine.EnableVideo();
        mRtcEngine.EnableVideoObserver();
        mRtcEngine.OnJoinChannelSuccess += OnJoinChannelSuccessHandler;
        mRtcEngine.OnLeaveChannel += OnLeaveChannelHandler;

        mRtcEngine.OnUserJoined += OnUserJoinedHandler;


    }
    void Join()
    {

        setUI(true);
        mRtcEngine.EnableVideo();
        mRtcEngine.EnableVideoObserver();
        myView.SetEnable(true);
        mRtcEngine.JoinChannelByKey(TOKEN, ChannelName, "", 0);
    }
    void Leave()
    {
        setUI(false);
        mRtcEngine.LeaveChannel();
        mRtcEngine.DisableVideo();
        mRtcEngine.DisableVideoObserver();

    }

    void OnJoinChannelSuccessHandler(string channelName, uint uid, int elapsed)
    {
        Debug.LogFormat("Joined Channel {0} successfully, my uid = {1}", channelName, uid);
    }

    void OnLeaveChannelHandler(RtcStats stats)
    {
        myView.SetEnable(false);
        if (remoteView != null)
        {
            remoteView.SetEnable(false);
        }
    }
    void OnUserJoinedHandler(uint uid, int elapsed)
    {
        if (userCount == 0)
        {
            if (remoteView == null)
            {
                remoteView = GameObject.Find("RemoteView").AddComponent<VideoSurface>();
            }

            remoteView.SetForUser(uid);
            remoteView.SetEnable(true);
            remoteView.SetVideoSurfaceType(AgoraVideoSurfaceType.RawImage);
            remoteView.SetGameFps(30);
            userCount++;
        }
        else
        {
            GameObject temp = Instantiate(test, new Vector3(0, 0, 0), Quaternion.identity);
            temp.name = "user" + userCount.ToString();
            userCount++;
            temp?.GetComponent<Button>()?.onClick.AddListener(changeObject);
            VideoSurface visited = temp.AddComponent<VideoSurface>();
            visited.SetForUser(uid);
            visited.SetEnable(true);
            visited.SetVideoSurfaceType(AgoraVideoSurfaceType.RawImage);
            visited.SetGameFps(30);

            GameObject faceContent = GameObject.Find("faceContent");
            temp.transform.SetParent(faceContent.transform);
        }
    }
    void OnUserOffline(uint uid, USER_OFFLINE_REASON reason)
    {
        remoteView.SetEnable(false);
    }

    void OnApplicationQuit()
    {
        if (mRtcEngine != null)
        {
            IRtcEngine.Destroy();
            mRtcEngine = null;
        }
    }
    private void makeVideoView(uint uid)
    {
        GameObject go = GameObject.Find(uid.ToString());
        if (!ReferenceEquals(go, null))
        {
            return; // reuse
        }

        // create a GameObject and assign to this new user
        VideoSurface videoSurface = makeImageSurface(uid.ToString());
        if (!ReferenceEquals(videoSurface, null))
        {
            // configure videoSurface
            videoSurface.SetForUser(uid);
            videoSurface.SetEnable(true);
            videoSurface.SetVideoSurfaceType(AgoraVideoSurfaceType.RawImage);
            videoSurface.SetGameFps(30);
        }
    }
    public VideoSurface makeImageSurface(string goName)
    {
        GameObject go = new GameObject();

        if (go == null)
        {
            return null;
        }

        go.name = goName;
        // to be renderered onto
        go.AddComponent<RawImage>();
        // make the object draggable
        GameObject canvas = GameObject.Find("RemoteView");
        if (canvas != null)
        {
            go.transform.parent = canvas.transform;
            Debug.Log("add video view");
        }
        else
        {
            Debug.Log("Canvas is null video view");
        }
        // set up transform
        go.transform.Rotate(0f, 0.0f, 180.0f);
        float xPos = Random.Range(Offset - Screen.width / 2f, Screen.width / 2f - Offset);
        float yPos = Random.Range(Offset, Screen.height / 2f - Offset);
        Debug.Log("position x " + xPos + " y: " + yPos);
        go.transform.localPosition = new Vector3(xPos, yPos, 0f);
        go.transform.localScale = new Vector3(3f, 4f, 1f);

        // configure videoSurface
        VideoSurface videoSurface = go.AddComponent<VideoSurface>();
        return videoSurface;
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void onFace()
    {
        Debug.Log("click");
    }

    void onClickAdd() // by태관  참여자가 추가됐을때 화면 오른쪽 리스트에 추가 시키는 함수
    {
        
        GameObject temp = Instantiate(test, new Vector3(0, 0, 0), Quaternion.identity);
        temp.name = "user" + userCount.ToString();
        userCount++;
        temp?.GetComponent<Button>()?.onClick.AddListener(changeObject);

        GameObject faceContent = GameObject.Find("faceContent");
        temp.transform.SetParent(faceContent.transform);
    }

    void setUI(bool boolean)
    {
        myFace.SetActive(boolean);
        remote.SetActive(boolean);
    }

    void changeObject() // by 태관 / 오른쪽 화면 리스트를 클릭시 실행되는 큰 화면과 교체하는 이벤트 리스너 함수
    {
        
        GameObject clickObject = EventSystem.current.currentSelectedGameObject;
        if (clickObject.name != "RemoteView"){ // 큰화면 클릭시 반응 x
            GameObject panel = GameObject.Find("Panel");
            RectTransform clickObject_T = clickObject.GetComponent<RectTransform>();

            // 클릭한 화면을 화면밖으로 빼돌림
            clickObject.transform.SetParent(panel.transform); 
            GameObject changeRemoteObject = GameObject.Find("RemoteView");
            RectTransform remoteView_T = changeRemoteObject.GetComponent<RectTransform>();


            clickObject_T.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, remoteView_T.rect.width);
            clickObject_T.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, remoteView_T.rect.height);
            clickObject.transform.position = changeRemoteObject.transform.position;

            changeRemoteObject.transform.SetParent(GameObject.Find("faceContent").transform);
            //remoteView_T.transform.position = new Vector3(0, 0, 0);
            remoteView_T.SetSizeWithCurrentAnchors(RectTransform.Axis.Horizontal, 100);
            remoteView_T.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, 100);

            
            string temp = changeRemoteObject.name;
            changeRemoteObject.name = clickObject.name;
            clickObject.name = temp;
        }
    }
}
