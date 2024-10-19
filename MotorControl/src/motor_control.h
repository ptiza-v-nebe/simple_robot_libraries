enum Direction { DIRECTION_FORWARD, DIRECTION_BACKWARD };
enum Motor { MOTOR_LEFT, MOTOR_RIGHT };


class MotorControl {
public:
  MotorControl(): enAPin(0), in1Pin(0), in2Pin(0), enBPin(0), in3Pin(0), in4Pin(0) 
  {
  }

  void begin(int enAPin, int in1Pin, int in2Pin, int enBPin, int in3Pin, int in4Pin){
      pinMode(enAPin, OUTPUT);
      pinMode(in1Pin, OUTPUT);
      pinMode(in2Pin, OUTPUT);
      pinMode(enBPin, OUTPUT);
      pinMode(in3Pin, OUTPUT);
      pinMode(in4Pin, OUTPUT);

      this->enAPin = enAPin;
      this->in1Pin = in1Pin;
      this->in2Pin = in2Pin;
      this->enBPin = enBPin;
      this->in3Pin = in3Pin;
      this->in4Pin = in4Pin;
  }

  // in percent 0-100
  void set_motor_speed(Motor motor, int raw_speed){
    int clipped_speed = clip_speed(raw_speed);
    if(clipped_speed > 0) {
      set_motor_direction(motor, DIRECTION_FORWARD);
    } else if (clipped_speed < 0 ) {
      set_motor_direction(motor, DIRECTION_BACKWARD);
    }

    int absolute_speed = abs(clipped_speed);

    int mapped_speed = 0;
    if(absolute_speed > 0){
      mapped_speed = map(absolute_speed, 0, 100, 90, 200);
    } else if (absolute_speed == 0) {
      mapped_speed = 0;
    }
    
    analogWrite(get_enable_pin(motor), mapped_speed);
  }

  void set_platform_speed(int longtitude_speed, int side_speed){
    set_motor_speed(MOTOR_LEFT, longtitude_speed + side_speed);
    set_motor_speed(MOTOR_RIGHT, longtitude_speed - side_speed);
  }

private:
  // Motor control pins : L298N H bridge
  int enAPin = 0; // Left motor PWM speed control
  int in1Pin = 0; // Left motor Direction 1
  int in2Pin = 0; // Left motor Direction 2

  int enBPin = 0; // Right motor PWM speed control
  int in3Pin = 0; // Right motor Direction 1
  int in4Pin = 0; // Right motor Direction 2


  void set_motor_direction(Motor motor, Direction direction){
    if(direction == DIRECTION_FORWARD){
      digitalWrite(get_direction_pin_1(motor), HIGH);
      digitalWrite(get_direction_pin_2(motor), LOW);
    } else if (direction == DIRECTION_BACKWARD){
      digitalWrite(get_direction_pin_1(motor), LOW);
      digitalWrite(get_direction_pin_2(motor), HIGH);
    }
  }

 int clip_speed(int speed_percent){
    int clipped_speed = speed_percent;
    if(speed_percent >= 100) {
      clipped_speed = 100;
    } else if (speed_percent <= -100) {
      clipped_speed = -100;
    }
    return clipped_speed;
  }

  int get_direction_pin_1(Motor motor){
    if(motor == MOTOR_RIGHT) {
      return in1Pin;
    } else if (motor == MOTOR_LEFT){
      return in3Pin;
    }
  }

  int get_direction_pin_2(Motor motor){
    if(motor == MOTOR_RIGHT) {
      return in2Pin;
    } else if (motor == MOTOR_LEFT){
      return in4Pin;
    }
  }

  int get_enable_pin(Motor motor){
    if(motor == MOTOR_RIGHT) {
      return enAPin;
    } else if (motor == MOTOR_LEFT){
      return enBPin;
    }
  }
};








