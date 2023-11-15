// Raspberry Pi 전용 코드
#ifdef RaspberryPi

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <lcd.h>
#include <wiringPi.h>
#include <wiringSerial.h>

// Serial 장치 위치
char device[] = "/dev/ttyUSB0";
int fd;
unsigned long baud = 9600;
unsigned long time = 0;

// 프로토타입 선언
void setup(void);
void loop(void);

// 설정 함수
void setup() {
    printf("%s \n", "Raspberry Startup!");
    fflush(stdout);

    // 파일 디스크립터 획득
    if ((fd = serialOpen(device, baud)) < 0) {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        exit(1); // 오류
    }

    // GPIO 설정
    if (wiringPiSetup() == -1) {
        fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
        exit(1); // 오류
    }
}

// 메인 함수
int main() {
    setup();

    if (wiringPiSetupGpio() == -1)
        return 1;

    int lcdHandle;
    char newChar;
    int count[4] = {0}; // 카운트 배열 초기화

    lcdHandle = lcdInit(2, 16, 4, 25, 24, 23, 22, 21, 17, 0, 0, 0, 0);

    while (1) {
        if (millis() - time >= 3000) {
            serialPuts(fd, "Pong!\n");
            serialPutchar(fd, 65); // ASCII 'A'
            time = millis();
        }

        if (serialDataAvail(fd)) {
            newChar = serialGetchar(fd);

            switch (newChar) {
                case '0':
                    printf("Cannot be\n");
                    count[0]++;
                    break;
                case '1':
                    printf("Can\n");
                    count[1]++;
                    break;
                case '2':
                    printf("Plastic\n");
                    count[2]++;
                    break;
                case '3':
                    printf("Glass\n");
                    count[3]++;
                    break;
            }

            for (int i = 0; i < 4; i++) {
                if (count[i] == 30) {
                    lcdClear(lcdHandle);
                    lcdPosition(lcdHandle, 0, 0);

                    switch (i) {
                        case 0:
                            lcdPuts(lcdHandle, "Cannot be");
                            lcdPosition(lcdHandle, 0, 1);
                            lcdPuts(lcdHandle, "      Recycled!");
                            break;
                        case 1:
                            lcdPuts(lcdHandle, "      Can");
                            lcdPosition(lcdHandle, 0, 1);
                            lcdPuts(lcdHandle, "Press 1st BTN");
                            break;
                        case 2:
                            lcdPuts(lcdHandle, "    Plastic");
                            lcdPosition(lcdHandle, 0, 1);
                            lcdPuts(lcdHandle, "Press 2nd BTN");
                            break;
                        case 3:
                            lcdPuts(lcdHandle, "    Glass");
                            lcdPosition(lcdHandle, 0, 1);
                            lcdPuts(lcdHandle, "Press 3rd BTN");
                            break;
                    }

                    count[i] = 0;
                }
            }
            fflush(stdout);
        }
    }

    return 0;
}

#endif // RaspberryPi
