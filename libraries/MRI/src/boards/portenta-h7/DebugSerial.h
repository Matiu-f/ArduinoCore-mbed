/* Copyright 2020 Adam Green (https://github.com/adamgreen/)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
// GDB Kernel debugging of the Arduino Portenta-H7 over a serial connection.
#pragma once

#include <Arduino.h>
#include <mbed.h>

extern "C" {
    typedef struct Token Token;
    void     mriPlatform_Init(Token* pParameterTokens);
    uint32_t mriPlatform_CommHasReceiveData(void);
    int      mriPlatform_CommReceiveChar(void);
    void     mriPlatform_CommSendChar(int character);
}


// UNDONE: Should I put the debugger objects into the Arduino namespace?
namespace arduino {

class DebugSerial {
public:
    // You must declare your DebugSerial object as a global.
    // Example:
    //      DebugSerial debugSerial(SERIAL1_TX, SERIAL1_RX, USART1_IRQn, 230400);
    DebugSerial(PinName txPin, PinName rxPin, IRQn_Type irq, uint32_t baudRate, bool breakInSetup=true);

    // You should never let your DebugSerial object go out of scope. Make it global or static. To warn you if you do
    // let it go out of scope by mistake, this destructor will break into GDB and then enter an infinite loop.
    ~DebugSerial();

protected:
    void        construct();
    void        setupStopInSetup();

    // These protected methods are called from global Platform* routines via singleton.
    void        setSerialPriority(uint8_t priority);
    uint32_t    hasReceiveData(void);
    int         receiveChar(void);
    void        sendChar(int character);

    void        initSerial();

    static int  justEnteredSetupCallback(void* pvContext);
    int         justEnteredSetup();

    typedef void(*IsrFunctionPtr)(void);

    mbed::UnbufferedSerial  _serial;
    IRQn_Type               _irq;
    bool                    _breakInSetup;

    friend void     ::mriPlatform_Init(Token* pParameterTokens);
    friend uint32_t ::mriPlatform_CommHasReceiveData(void);
    friend int      ::mriPlatform_CommReceiveChar(void);
    friend void     ::mriPlatform_CommSendChar(int character);
};

} // namespace arduino
