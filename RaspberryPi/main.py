from Packet import Packet
from robot.leg import Leg
from serial.tools import list_ports
from serial import Serial, SerialException
from sys import argv
from time import sleep
from time import time


def main(args):
    sample_packet = Packet()

    leg_1 = Leg()
    leg_2 = Leg()
    leg_3 = Leg()
    leg_4 = Leg()
    leg_5 = Leg()

    leg_1.set_angles(30, 20, 20)
    leg_2.set_angles(45, 60, 130)
    leg_3.set_angles(30, 43, 31)
    leg_4.set_angles(19, 41, 42)
    leg_5.set_angles(32, 20, 90)

    sample_packet.set_leg(1, leg_1)
    sample_packet.set_leg(3, leg_2)
    sample_packet.set_leg(4, leg_3)
    sample_packet.set_leg(5, leg_4)
    sample_packet.set_leg(6, leg_5)

    sample_packet.make_packet()

    # TODO Make a rule on raspberry pi for Arduino, so that the port always stays the same.
    # Configure the baud rate of the connection.
    baud_rate = 9600

    for i in range(len(args)):
        mode = args[i]
        if mode == '-d':
            print('[LOG]: This program is in debug mode.')
        elif mode == '-br':
            print('[LOG]: The baud rate is set to ' + args[i+1])
            baud_rate = args[i+1]
        elif mode == '-p':
            print('[LOG]: The Arduino port was set to ' + args[i+1])
            port = args[i+1]

    for port in (list_ports.comports()):
        print(port[0])
        try:
            ser = Serial(port[0], baud_rate, timeout=2)
            start_time = int(round(time()))
            sleep(3)  # wait for the device to be ready
            # send hello command
            for character in str(sample_packet):
                ser.write(bytes([ord(character)]))
                print(ord(bytes([ord(character)])))

            print("------------------------")

            while True:
                current_time = int(round(time()))
                if current_time - start_time < 60:
                    incoming_byte = ser.read()
                    if incoming_byte:
                        if ord(incoming_byte) == 253:
                            print('[Log]: Received valid packet.')
                        elif ord(incoming_byte) == 251:
                            print('[Log]: Detected terminal character.')
                        elif ord(incoming_byte) == 252:
                            print('[Warning]: Exceeded possible packet size.')
                        elif ord(incoming_byte) >= 200 and ord(incoming_byte) <= 216 :
                            print('[Log]: Byte ' + str(ord(incoming_byte) - 200) + ':')
                        elif ord(incoming_byte) == 222:
                            print('[Log]: First or third initiation character found.')
                        elif ord(incoming_byte) == 230:
                            print('[Log]: Second initiation character found.')
                        else:
                            print(ord(incoming_byte))
                else:
                    ser.close()
                    return


        except SerialException:
            # print("opening serial failed")
            print("[ERROR]: Connection Failure.")
            pass



if __name__ == "__main__":
    main(argv)
