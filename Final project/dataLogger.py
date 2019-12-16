import  serial
import  time
import  csv

ser = serial.Serial('/dev/ttyACM0', 115200)
ser.flushOutput()
ser.flushInput()

while True:
        str = ser.read(256);
        tempPosition = str.find("Temperture = ");
        if tempPosition != -1:
                end = str.find(",", tempPosition)
                if end != -1:
                        temperature = str[tempPosition + 13 : end];
                        print("Temperature : "+ temperature);
                        with open("data.csv", "a") as f:
                                writer = csv.writer(f, delimiter=",")
                                writer.writerow([time.time(),"Temperature : ", temperature.strip()])
        humidityPos = str.find("Humidity = ")
        if humidityPos != -1:
                end = str.find(",", humidityPos)
                if end !=  -1:
                        humidity = str[humidityPos + 11:end]
                        print("Humidity : " + humidity);
                        with open("data.csv", "a") as f:
                                writer = csv.writer(f, delimiter=",")
                                writer.writerow([time.time(),"Humidity    : ", humidity.strip()])
