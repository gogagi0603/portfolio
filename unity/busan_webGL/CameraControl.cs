using System.Collections;
using System.Collections.Generic;
using UnityEngine;
//by �°� / ���ͺ� ī�޶� ȸ�� �� �� �Դϴ�. player�� �Ĵٺ� player�� �־��ְ� camera�� script�� �ְ� �����ϸ� �˴ϴ�
public class CameraControl : MonoBehaviour
{
    private Quaternion TargetRotation;  // ���������� ������ Gap�� �� ������ �����.

    private float ZoomSpeed = 1;            // �� ���ǵ�.
    private Vector3 AxisVec;           // ���� ����.
    private Vector3 Gap;               // ȸ�� ���� ��.
    private Transform MainCamera;      // ī�޶� ������Ʈ.
    public GameObject player; // �ٶ� �÷��̾� ������Ʈ�Դϴ�.
    private float xmove = 0;  // X�� ���� �̵���
    private float ymove = 0;  // Y�� ���� �̵���
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
        //  ���콺 ��Ŭ�� �߿��� ī�޶� ���� ����
        if (Input.GetMouseButton(1))
        {
            xmove += Input.GetAxis("Mouse X"); // ���콺�� �¿� �̵����� xmove �� �����մϴ�.
            ymove -= Input.GetAxis("Mouse Y"); // ���콺�� ���� �̵����� ymove �� �����մϴ�.
        }
        transform.rotation = Quaternion.Euler(ymove, xmove, 0); // �̵����� ���� ī�޶��� �ٶ󺸴� ������ �����մϴ�.
        Vector3 reverseDistance = new Vector3(0.0f, 0.0f, distance); // ī�޶� �ٶ󺸴� �չ����� Z ���Դϴ�. �̵����� ���� Z ������� ���͸� ���մϴ�.
        transform.position = player.transform.position - transform.rotation * reverseDistance; // �÷��̾��� ��ġ���� ī�޶� �ٶ󺸴� ���⿡ ���Ͱ��� ������ ��� ��ǥ�� �����մϴ�.
    
    }

    // Update is called once per frame
    void Update()
    {
        zoom();
        rotate();
    }
}