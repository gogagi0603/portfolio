using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
public class Enemy : MonoBehaviour
{
    NavMeshAgent agent;
    public Transform[] waypoints;
    int waypointIndex;
    Vector3 target;

    // Start is called before the first frame update
    private void Awake()
    {
    }
    void Start()
    {
        
        agent = GetComponent<NavMeshAgent>();
        Debug.Log(agent);
        UpdateDestination();
    }
    

    // Update is called once per frame
    void Update()
    {
        //Debug.Log(target);

       
            if (Vector2.Distance(new Vector2(transform.position.x,transform.position.z), new Vector2(target.x, target.z)) < 1)
            {
                IterateWaypointIndex();
                UpdateDestination();

            
            }
        
    }
    public void UpdateDestination()
    {
        target =waypoints[waypointIndex].position;
        agent.SetDestination(target);

    }

    public void IterateWaypointIndex()
    {
        waypointIndex++;
        if (waypointIndex == waypoints.Length)
        {
            waypointIndex = 0;
        }
    }
}
