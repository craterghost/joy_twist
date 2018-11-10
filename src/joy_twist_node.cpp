//http://adafruit.github.io/Adafruit-PWM-Servo-Driver-Library/html/class_adafruit___p_w_m_servo_driver.html
#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
//#include <Adafruit_PWMServoDriver.h>

 class JoyTeleop
 {
	public:
		JoyTeleop();
	private:
		void joyCallback(const sensor_msgs::Joy::ConstPtr &msg);


		double linearScale, angularScale;
		int deadmanButton, linearAxis, angularAxis;
		bool canMove;
		ros::Subscriber joySub;
		ros::Publisher twistPub;
		ros::NodeHandle nh;
		ros::Timer timeout;
    geometry_msgs::Twist twistMsg;

};

JoyTeleop::JoyTeleop() {
	joySub = nh.subscribe("/joy", 1000, &JoyTeleop::joyCallback, this);
	twistPub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);

}

void JoyTeleop::joyCallback(const sensor_msgs::Joy::ConstPtr &msg) {
  //deadmanButton
  if(msg->buttons[0]==0){
	// process and publish
  twistMsg.linear.x = msg->axes[2];                                             //thrust
  twistMsg.linear.y = msg->axes[0];                                             //height
  twistMsg.linear.z = msg->axes[1];                                             //height
  twistMsg.angular.z = msg->axes[3];                                            //gier

  if(msg->buttons[3]==1){
    twistMsg.angular.y = 0.5;
  }else if (msg->buttons[4]==1){
    twistMsg.angular.y = -0.5;
  }else{
    twistMsg.angular.y = 0;
  }
	twistPub.publish(twistMsg);
  }
}



int main(int argc, char** argv) {
	ros::init(argc, argv, "joy_twist_rov");
	JoyTeleop joy_teleop_node;

	ros::spin();

	return 0;
}
