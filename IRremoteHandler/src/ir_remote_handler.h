#include <IRremote.hpp> // Make sure to include the necessary library for IR
#define DECODE_NEC


class IRRemoteHandler {
    using CommandHandler = void(*)(uint8_t command);
    using ButtonHandler = void(*)();
public:
    IRRemoteHandler() : lastCommand(0), lastReceiveTime(0), buttonPressed(false) {}

    void begin(int ir_receive_pin) {
        IrReceiver.begin(ir_receive_pin, ENABLE_LED_FEEDBACK); // Initialize the IR receiver
    }

    void run_once() {
        if (IrReceiver.decode()) {
            if (commandHandler) {
                commandHandler(IrReceiver.decodedIRData.command);
            }
            handleRepeatSignal();
            IrReceiver.resume(); // Prepare for the next value
        }

        checkButtonRelease();
    }

    void set_command_handler(CommandHandler handler) {
        commandHandler = handler;
    }

    void set_pressed_handler(ButtonHandler handler) {
        pressedHandler = handler;
    }

    void set_released_handler(ButtonHandler handler) {
        releaseHandler = handler;
    }

private:
    CommandHandler commandHandler = nullptr;
    ButtonHandler pressedHandler = nullptr;
    ButtonHandler releaseHandler = nullptr;
    uint8_t lastCommand;
    unsigned long lastReceiveTime;
    bool buttonPressed;
    const unsigned long debounceDelay = 150; // Adjust as needed

    void handleRepeatSignal() {
        if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
            if (IrReceiver.decodedIRData.command == lastCommand) {
                lastReceiveTime = millis();
            }
        } else {
          // New button press detected
          lastCommand = IrReceiver.decodedIRData.command;
          lastReceiveTime = millis();
          buttonPressed = true;
          if(pressedHandler){
            pressedHandler();  
          }
        }
    }

    void checkButtonRelease() {
        if (buttonPressed && (millis() - lastReceiveTime > debounceDelay)) {
            buttonPressed = false;
            if(releaseHandler){
              releaseHandler();
            }
        }
    }
};
