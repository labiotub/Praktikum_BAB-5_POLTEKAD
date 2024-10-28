import serial
import time
import firebase_admin
from firebase_admin import credentials, db

# Path to your Firebase service account key file (Replace this with your actual path)
cred = credentials.Certificate('/path/to/your/serviceAccountKey.json')

# Initialize the Firebase app with the service account and the Firebase Realtime Database URL
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://your-database-name.firebaseio.com/'  # Replace with your actual Firebase Realtime Database URL
})

# Firebase references for motor mode and angle
mode_ref = db.reference('motor_control/mode')  # Mode (0 for Servo, 1 for Stepper)
angle_ref = db.reference('motor_control/angle')  # Angle (0-100)

# Set up the serial connection for Serial0 (GPIO pins 14 TXD and 15 RXD)
ser = serial.Serial('/dev/serial0', 9600, timeout=1)  # Use '/dev/serial0' for GPIO UART communication
time.sleep(2)  # Allow some time for the serial connection to establish

def send_command(command):
    # Send command to Arduino
    ser.write((command + '\n').encode())
    time.sleep(0.1)  # Small delay to ensure data is sent/received correctly

    # Read and print Arduino's response
    while ser.in_waiting > 0:
        print(ser.readline().decode('utf-8').strip())

# Function to control motors using Firebase
def control_motor_from_firebase():
    # Get initial mode and angle from Firebase
    mode = mode_ref.get()
    angle = angle_ref.get()

    # Send initial mode and angle commands to Arduino
    send_command(f"mode:{mode}")
    send_command(f"angle:{angle}")

    print(f"Initial mode: {mode}, angle: {angle}")

    # Continuously listen for changes in Firebase values
    while True:
        # Get the updated mode and angle from Firebase
        new_mode = mode_ref.get()
        new_angle = angle_ref.get()

        # Check if the mode has changed
        if new_mode != mode:
            mode = new_mode
            send_command(f"mode:{mode}")
            print(f"Switched to {'Servo' if mode == 0 else 'Stepper'} mode")

        # Check if the angle has changed
        if new_angle != angle:
            angle = new_angle
            send_command(f"angle:{angle}")
            print(f"Set angle to {angle}")

        time.sleep(0.5)  # Polling interval to check for updates

# Example of controlling motors based on Firebase input
control_motor_from_firebase()

# Close the serial port when done
ser.close()
