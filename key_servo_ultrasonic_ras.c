#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

#define KEYPAD_PB1 2
#define KEYPAD_PB2 3
#define KEYPAD_PB3 20

#define SERVO1 17
#define SERVO2 21

#define TP 13
#define EP 19

#define MAX_KEY_BT_NUM 3

const int KeypadTable[3] = {KEYPAD_PB1, KEYPAD_PB2, KEYPAD_PB3};

int KeypadRead(void) {
    int i, nKeypadstate;
    nKeypadstate = 0;
    for (i = 0; i < MAX_KEY_BT_NUM; i++) {
        if (!digitalRead(KeypadTable[i])) {
            nKeypadstate |= (1 << i);
        }
    }
    return nKeypadstate;
}

float getDistance(void) {
    float fDistance;
    int nStartTime, nEndTime;

    digitalWrite(TP, LOW);
    delayMicroseconds(2);

    digitalWrite(TP, HIGH);
    delayMicroseconds(10);
    digitalWrite(TP, LOW);

    while (digitalRead(EP) == LOW);
    nStartTime = micros();

    while (digitalRead(EP) == HIGH);
    nEndTime = micros();

    fDistance = (nEndTime - nStartTime) * 0.034 / 2.;

    return fDistance;
}

int main() {
    if (wiringPiSetupGpio() == -1) return 1;

    int i;
    int nKeypadstate;
    int fDistance = 0;
    softPwmCreate(SERVO1, 0, 200);
    softPwmCreate(SERVO2, 0, 200);

    for (i = 0; i < MAX_KEY_BT_NUM; i++) {
        pinMode(KeypadTable[i], INPUT);
    }

    while (1) {
        nKeypadstate = KeypadRead();
        for (i = 0; i < MAX_KEY_BT_NUM; i++) {
            if ((nKeypadstate & (1 << i))) {
                if (i == 0) {  // 1
                    printf("1\n");
                    delay(500);

                    fDistance = getDistance();
                    printf("%dcm\n", fDistance);
                    delay(500);
                    if (fDistance < 40) {
                        softPwmWrite(SERVO1, 25); // 반시계 방향
                        delay(300);
                        softPwmWrite(SERVO1, 15);
                        delay(1000);
                        softPwmWrite(SERVO1, 5); // 시계 방향
                        delay(300);

                        softPwmWrite(SERVO1, 15);
                        delay(1000);
                    }
                } else if (i == 1) {
                    printf("2\n");
                    delay(500);

                    fDistance = getDistance();
                    printf("%dcm\n", fDistance);
                    delay(500);
                    if (fDistance < 40) {
                        softPwmWrite(SERVO1, 5); // 반시계 방향
                        delay(300);

                        softPwmWrite(SERVO1, 15);
                        delay(1000);

                        softPwmWrite(SERVO1, 25); // 시계 방향
                        delay(300);

                        softPwmWrite(SERVO1, 15);
                        delay(500);

                        softPwmWrite(SERVO2, 25); // 반시계 방향
                        delay(300);

                        softPwmWrite(SERVO2, 15);
                        delay(1000);

                        softPwmWrite(SERVO2, 5); // 시계 방향
                        delay(300);

                        softPwmWrite(SERVO2, 15);
                        delay(1000);
                    }
                } else if (i == 2) {  //
                    printf("3\n");
                    delay(500);

                    fDistance = getDistance();
                    printf("%dcm\n", fDistance);
                    delay(500);

                    if (fDistance < 40) {
                        softPwmWrite(SERVO1, 5); // 반시계 방향
                        delay(300);

                        softPwmWrite(SERVO1, 15);
                        delay(1000);

                        softPwmWrite(SERVO1, 25); // 시계 방향
                    }
                }
            }
        }
    }

    return 0;
}
