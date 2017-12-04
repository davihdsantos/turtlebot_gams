
#ifndef   _PLATFORM_TURTLEBOT_PLATFORM_H_
#define   _PLATFORM_TURTLEBOT_PLATFORM_H_


#ifdef _GAMS_ROS_

#include "gams/platforms/ros/RosBase.h"
#include "threads/TopicListener.h"

#include "gams/variables/Self.h"
#include "gams/variables/Sensor.h"
#include "gams/variables/PlatformStatus.h"
#include "gams/utility/Position.h"
#include <gams/pose/GPSFrame.h>
#include "gams/platforms/PlatformFactory.h"
#include "madara/knowledge/KnowledgeBase.h"
#include "madara/threads/Threader.h"
#include <madara/knowledge/containers/NativeDoubleVector.h>


#include "ros/ros.h"
#include "move_base_msgs/MoveBaseAction.h"
#include "actionlib/client/simple_action_client.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include <dynamic_reconfigure/DoubleParameter.h>
#include <dynamic_reconfigure/Reconfigure.h>
#include <dynamic_reconfigure/Config.h>
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>
#include <sensor_msgs/LaserScan.h>

namespace platforms
{        
  /**
  * A custom platform generated by gpc.pl
  **/
  class turtlebot_platform : public gams::platforms::RosBase
  {
//  protected: 
	//int status = gams::platforms::UNKNOWN;
  public:
    /**
     * Constructor
     * @param  knowledge  context containing variables and values
     * @param  sensors    map of sensor names to sensor information
     * @param  self       self referencing variables for the agent
     **/
    turtlebot_platform (
      madara::knowledge::KnowledgeBase * knowledge = 0,
      gams::variables::Sensors * sensors = 0,
      gams::variables::Self * self = 0);

    /**
     * Destructor
     **/
    virtual ~turtlebot_platform ();

    /**
     * Polls the sensor environment for useful information. Required.
     * @return number of sensors updated/used
     **/
    virtual int sense (void);

    /**
     * Analyzes platform information. Required.
     * @return bitmask status of the platform. @see PlatformAnalyzeStatus.
     **/
    virtual int analyze (void);

    /**
     * Gets the name of the platform. Required.
     **/
    virtual std::string get_name () const;

    /**
     * Gets the unique identifier of the platform. This should be an
     * alphanumeric identifier that can be used as part of a MADARA
     * variable (e.g. vrep_ant, autonomous_snake, etc.) Required.
     **/
    virtual std::string get_id () const;

    /**
     * Gets the position accuracy in meters. Optional.
     * @return position accuracy
     **/
    virtual double get_accuracy (void) const;

    /**
     * Gets Location of platform, within its parent frame. Optional.
     * @return Location of platform
     **/
    gams::pose::Position get_location () const;

    /**
     * Gets Rotation of platform, within its parent frame. Optional.
     * @return Location of platform
     **/
    gams::pose::Orientation get_orientation () const;

    /**
     * Gets sensor radius. Optional.
     * @return minimum radius of all available sensors for this platform
     **/
    virtual double get_min_sensor_range () const;

    /**
     * Gets move speed. Optional.
     * @return speed in meters per second
     **/
    virtual double get_move_speed () const;

    /**
     * Instructs the agent to return home. Optional.
     * @return the status of the home operation, @see PlatformReturnValues
     **/
    virtual int home (void);

    /**
     * Instructs the agent to land. Optional.
     * @return the status of the land operation, @see PlatformReturnValues
     **/
    virtual int land (void);

    /**
     * Moves the platform to a location. Optional.
     * @param   target    the coordinates to move to
     * @param   epsilon   approximation value
     * @return the status of the move operation, @see PlatformReturnValues
     **/
    virtual int move (const gams::pose::Position & location,
      double epsilon = 0.1);

    /**
     * Rotates the platform to match a given angle. Optional.
     * @param   target    the rotation to move to
     * @param   epsilon   approximation value
     * @return the status of the rotate, @see PlatformReturnValues
     **/
    virtual int rotate (const gams::pose::Orientation & target,
      double epsilon = M_PI/16);

    /**
     * Moves the platform to a pose (location and rotation). Optional.
     *
     * This default implementation calls move and rotate with the
     * Location and Rotation portions of the target Pose. The return value
     * is composed as follows: if either call returns ERROR (0), this call
     * also returns ERROR (0). Otherwise, if BOTH calls return ARRIVED (2),
     * this call also returns ARRIVED (2). Otherwise, this call returns
     * MOVING (1)
     *
     * Overrides might function differently.
     *
     * @param   target        the coordinates to move to
     * @param   loc_epsilon   approximation value for the location
     * @param   rot_epsilon   approximation value for the rotation
     * @return the status of the operation, @see PlatformReturnValues
     **/
    virtual int pose (const gams::pose::Pose & target,
      double loc_epsilon = 0.1, double rot_epsilon = M_PI/16);

    /**
     * Pauses movement, keeps source and dest at current values. Optional.
     **/
    virtual void pause_move (void);

    /**
     * Set move speed. Optional.
     * @param speed new speed in meters/second
     **/
    virtual void set_move_speed (const double& speed);

    /**
     * Stops movement, resetting source and dest to current location.
     * Optional.
     **/
    virtual void stop_move (void);

    /**
     * Instructs the agent to take off. Optional.
     * @return the status of the takeoff, @see PlatformReturnValues
     **/
    virtual int takeoff (void);
    
    /**
     * Returns the reference frame for the platform (e.g. GPS or cartesian)
     **/
    virtual const gams::pose::ReferenceFrame & get_frame (void) const;

    std::string getMoveBaseStatus() { return move_client_.getState().toString(); };

    void cleanAllStatus();
    

    void set_home(gams::pose::Position home);


  private:
    // a threader for managing platform threads
    madara::threads::Threader threader_;
    
    // a default GPS frame
    //static gams::pose::GPSFrame  gps_frame;
    
    // location received by topic ODOM
    //gams::pose::Position location_;

    // orientation received by topic ODOM
    //gams::pose::Orientation orientation_;


    gams::pose::Position home_;

    gams::pose::Position targetLocation_;

    // a default Cartesian frame
    static gams::pose::CartesianFrame  cartesian_frame;

	const std::string ros_namespace_;

    ros::NodeHandle node_handle_;
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> move_client_;

    bool firstMoveSent;
    ros::ServiceClient updateServiceClientMoveBase;

    madara::knowledge::containers::Double moveSpeed_;
    madara::knowledge::containers::Double min_sensor_range_;
    madara::knowledge::containers::Double max_sensor_range_;
    //madara::knowledge::containers::NativeDoubleVector location_;
    madara::knowledge::containers::NativeDoubleVector orientation_;

    std::map<std::string, std::string> remap;

  }; // end turtlebot_platform class


    

  /**
   * A factory class for creating turtlebot_platform platforms
   **/
  class turtlebot_platformFactory : public gams::platforms::PlatformFactory
  {
  public:
    /**
     * Creates a turtlebot_platform platform.
     * @param   args      no arguments are necessary for this platform
     * @param   knowledge the knowledge base. This will be set by the
     *                    controller in init_vars.
     * @param   sensors   the sensor info. This will be set by the
     *                    controller in init_vars.
     * @param   platforms status inform for all known agents. This
     *                    will be set by the controller in init_vars
     * @param   self      self-referencing variables. This will be
     *                    set by the controller in init_vars
     **/
    virtual gams::platforms::BasePlatform * create (
      const madara::knowledge::KnowledgeMap & args,
      madara::knowledge::KnowledgeBase * knowledge,
      gams::variables::Sensors * sensors,
      gams::variables::Platforms * platforms,
      gams::variables::Self * self);
  };
    
} // end platforms namespace

#endif // _GAMS_ROS_

#endif // _PLATFORM_TURTLEBOT_PLATFORM_H_
