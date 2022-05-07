using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//by 태관 / 사용자 움직임. 걷기 뛰기 점프

public class Player : MonoBehaviour
{
    public float speed;
    float hAxis;
    float vAxis;
    Vector3 moveVec;
    // Start is called before the first frame update
    Animator anim;
    Rigidbody rigid;
    bool wDown;
    bool isJump;
    bool jDown;
    bool isDodge;
    Vector3 dodgeVec;

    void Awake()
    {
        anim = GetComponentInChildren<Animator>();
    }

    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        GetInput();
        Move();
        Turn();
        Jump();
        Dodge();

    }
    void GetInput()
    {
        hAxis = Input.GetAxisRaw("Horizontal");
        vAxis = Input.GetAxisRaw("Vertical");
        wDown = Input.GetButton("Walk");
        jDown = Input.GetButtonDown("Jump");
        
    }
    void Move()
    {
        moveVec = new Vector3(hAxis, 0, vAxis).normalized;

        if (isDodge)
        {
            moveVec = dodgeVec;
        }

        if (wDown)
        {
            transform.position += moveVec * speed * 0.3f * Time.deltaTime;
        }
        else
        {
            transform.position += moveVec * speed * Time.deltaTime;
        }
        anim.SetBool("isRun", moveVec != Vector3.zero);
        anim.SetBool("isWalk", wDown);
    }

    void Turn()
    {
        transform.LookAt(transform.position + moveVec);
    }

    void Jump()
    {
        if (jDown && !isJump && moveVec == Vector3.zero)
        {
            rigid.AddForce(Vector3.up * 15, ForceMode.Impulse);
            isJump = true;
            anim.SetBool("isJump", true);
            anim.SetTrigger("doJump");
        }
    }

    void Dodge()
    {
        if (jDown && !isJump && moveVec != Vector3.zero && !isDodge)
        {
            dodgeVec = moveVec;
            speed *= 2;
            anim.SetTrigger("doDodge");
            isDodge = true;

            Invoke("DodgeOut", 0.4f);
        }
    }

    void DodgeOut()
    {
        speed *= 0.5f;
        isDodge = false;
    }

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.tag == "Floor")
        {
            anim.SetBool("isJump", false);
            isJump = false;
        }
    }

    void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Weapon"))
        {
           
        }
    }

    void OnTriggerExit(Collider other)
    {
        if (other.CompareTag("Weapon"))
        {
            
        }
    }

}
