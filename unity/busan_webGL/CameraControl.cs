using System.Collections;
using System.Collections.Generic;
using UnityEngine;
//by 태관 / 쿼터뷰 카메라 회전 및 줌 입니다. player에 쳐다볼 player를 넣어주고 camera에 script를 넣고 실행하면 됩니다
public class CameraControl : MonoBehaviour
{
    private Quaternion TargetRotation;  // 최종적으로 축적된 Gap이 이 변수에 저장됨.

    private float ZoomSpeed = 1;            // 줌 스피드.
    private Vector3 AxisVec;           // 축의 벡터.
    private Vector3 Gap;               // 회전 축적 값.
    private Transform MainCamera;      // 카메라 컴포넌트.
    public GameObject player; // 바라볼 플레이어 오브젝트입니다.
    private float xmove = 0;  // X축 누적 이동량
    private float ymove = 0;  // Y축 누적 이동량
    private float distance = 20;

    void Start()
    {
        MainCamera = transform;
    }
    void zoom()
    {
        if (Input.GetAxis("Mouse ScrollWheel") != 0)
        {
            distance += Input.GetAxis("Mouse ScrollWheel") * ZoomSpeed * -1;

            AxisVec = transform.forward * -1;

            if (Input.GetAxis("Mouse ScrollWheel") > 0)
            {
                if (distance <= 10f)
                {
                    distance = 10f;
                }
                else
                {
                    AxisVec *= (distance * -1);
                    transform.position = MainCamera.position + AxisVec;
                }
            }
            else
            {
                if (distance >= 40f)
                { 
                    distance = 40f;
                }
                else
                {
                    AxisVec *= distance;
                    transform.position = MainCamera.position + AxisVec;

                }
            }
        }
    }

    void rotate()
    {
        //  마우스 우클릭 중에만 카메라 무빙 적용
        if (Input.GetMouseButton(1))
        {
            xmove += Input.GetAxis("Mouse X"); // 마우스의 좌우 이동량을 xmove 에 누적합니다.
            ymove -= Input.GetAxis("Mouse Y"); // 마우스의 상하 이동량을 ymove 에 누적합니다.
        }
        transform.rotation = Quaternion.Euler(ymove, xmove, 0); // 이동량에 따라 카메라의 바라보는 방향을 조정합니다.
        Vector3 reverseDistance = new Vector3(0.0f, 0.0f, distance); // 카메라가 바라보는 앞방향은 Z 축입니다. 이동량에 따른 Z 축방향의 벡터를 구합니다.
        transform.position = player.transform.position - transform.rotation * reverseDistance; // 플레이어의 위치에서 카메라가 바라보는 방향에 벡터값을 적용한 상대 좌표를 차감합니다.
    
    }

    // Update is called once per frame
    void Update()
    {
        zoom();
        rotate();
    }
}