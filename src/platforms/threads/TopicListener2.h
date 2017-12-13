
#ifndef   _PLATFORM_THREAD_TOPIC_LISTENER2_H_
#define   _PLATFORM_THREAD_TOPIC_LISTENER2_H_

#include <string>

#include "madara/threads/BaseThread.h"
#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/LaserScan.h>
#include "madara/knowledge/KnowledgeBase.h"
#include <madara/knowledge/containers/Double.h>
#include <madara/knowledge/containers/NativeDoubleVector.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>
#include <move_base_msgs/MoveBaseActionFeedback.h>
#include <gams/variables/Self.h>
#include <gams/variables/PlatformStatus.h>
#include <actionlib_msgs/GoalStatusArray.h>
#include <gams/pose/Position.h>
#include <gams/utility/GPSPosition.h>


namespace platforms
{
  namespace threads
  {
    /**
    * A custom thread generated by gpc.pl
    **/
    class TopicListener2 : public madara::threads::BaseThread
    {
    public:
      /**
       * Default constructor
       **/
      TopicListener2 (ros::NodeHandle node_handle, gams::variables::Self* self, gams::variables::PlatformStatus status);
      
      /**
       * Destructor
       **/
      virtual ~TopicListener2 ();
      
      /**
        * Initializes thread with MADARA context
        * @param   context   context for querying current program state
        **/
      virtual void init (madara::knowledge::KnowledgeBase & knowledge);

      /**
        * Executes the main thread logic
        **/
      virtual void run (void);

      void processOdom(const nav_msgs::Odometry::ConstPtr& odom);

      void processScanOnce(const sensor_msgs::LaserScan::ConstPtr& scan);

      void processFeedback(const move_base_msgs::MoveBaseActionFeedback::ConstPtr& feed);

      void processStatus(const actionlib_msgs::GoalStatusArray::ConstPtr& status);


      void cleanAllStatus(void);
    private:
      /// data plane if we want to access the knowledge base
      madara::knowledge::KnowledgeBase data_;
      bool odomChanged_;
      bool scanChanged_;

      ros::NodeHandle node_handle_;

      tf::TransformListener listener;

      ros::Subscriber subOdom_;
      ros::Subscriber subScan_;
      ros::Subscriber subFeed_;
      ros::Subscriber subStatus_;

      gams::variables::Self* self_;
      gams::variables::PlatformStatus status_;


      madara::knowledge::containers::Integer goalId_;
      madara::knowledge::containers::String frame_;
      madara::knowledge::containers::Double initial_lat_;
      madara::knowledge::containers::Double initial_lon_;
      madara::knowledge::containers::Double initial_alt_;
	  madara::knowledge::containers::Double min_sensor_range_;
	  madara::knowledge::containers::Double max_sensor_range_;
	  madara::knowledge::containers::String frameType_;
    };
  } // end namespace threads
} // end namespace platforms

#endif // _PLATFORM_THREAD_TOPIC_LISTENER2_H_
