#ifndef PX4_FC_COMMS_HPP
#define PX4_FC_COMMS_HPP

////////////////////////////////////////////////////////////////////////////
//
// PX4 flight controller node.
//
// Implements node behaviours specific to PX4 FC
//
////////////////////////////////////////////////////////////////////////////
#include <ros/ros.h>
#include <string>

#include "CommonConf.hpp"
#include "MspConf.hpp"

#include "iarc7_msgs/BoolStamped.h"
#include "iarc7_msgs/Float64Stamped.h"
#include "iarc7_msgs/OrientationThrottleStamped.h"

namespace FcComms
{
    // Inherit from CommonFcComms to get the stuff any Fc is required to do.
    class PX4FcComms
    {
    public:
        PX4FcComms();
        ~PX4FcComms() = default;

        // Used to find and connect to the serial port
        FcCommsReturns __attribute__((warn_unused_result))
            connect();

        // Disconnect from FC, should be called before destructor.
        FcCommsReturns  __attribute__((warn_unused_result))
            disconnect();

        // Handle periodically updating polled info.
        FcCommsReturns  __attribute__((warn_unused_result))
            handleComms();

        // Find out if the FC is armed.
        FcCommsReturns  __attribute__((warn_unused_result))
            isArmed(bool& armed);

        // Perform any post arming actions
        FcCommsReturns  __attribute__((warn_unused_result))
            postArm(bool arm);

        // Calibrate the accelerometer
        FcCommsReturns  __attribute__((warn_unused_result))
            calibrateAccelerometer();

        // Find out if the FC is in failsafe
        FcCommsReturns  __attribute__((warn_unused_result))
            isFailsafe(bool& failsafe);

        // Get the battery voltage of the FC.
        FcCommsReturns  __attribute__((warn_unused_result))
            getBattery(float& voltage);

        // Get the attitude of the FC in the order roll pitch yaw
        FcCommsReturns  __attribute__((warn_unused_result))
            getAttitude(double (&attitude)[3]);

        // Getter for current connection status
        inline const FcCommsStatus& getConnectionStatus() const
        {
            return fc_comms_status_;
        }

        // Send the flight controller RX values
        FcCommsReturns  __attribute__((warn_unused_result))
            processDirectionCommandMessage(
                const iarc7_msgs::OrientationThrottleStamped::ConstPtr& message);
        FcCommsReturns  __attribute__((warn_unused_result))
            setArm(bool arm);

        // Get the acceleration in m/s^2 and the angular velocities in rad/s
        FcCommsReturns  __attribute__((warn_unused_result))
            getIMU(double (&accelerations)[3], double (&angular_velocities)[3]);

        FcCommsReturns  __attribute__((warn_unused_result))
            isAutoPilotAllowed(bool& allowed);

        FcCommsReturns  __attribute__((warn_unused_result))
            safetyLand();
        
    private:
        // Don't allow the copy constructor or assignment.
        PX4FcComms(const PX4FcComms& rhs) = delete;
        PX4FcComms& operator=(const PX4FcComms& rhs) = delete;

        // State of communication with flight controller
        FcCommsStatus fc_comms_status_ = FcCommsStatus::kDisconnected;

        // FC implementation specific to hold intermediate rc values
        uint16_t translated_rc_values_[8]{0};
    };
} // End namspace

#endif
