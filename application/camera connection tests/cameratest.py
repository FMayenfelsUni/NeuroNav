import socket

def send_message(ip_address, port, message):
    try:
        # Create a socket object
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            # Connect to the server
            s.connect((ip_address, port))
            
            # Send the message
            s.sendall(message.encode())
            
            # Receive data from the server
            data = s.recv(1024)
            
            # Print the received data
            print("Received:", data.decode())
    except Exception as e:
        print("An error occurred:", e)

if __name__ == "__main__":
    ip_address = "131.246.110.186"
    port = 8765
    message = "telnet P9-11701.local 8765\nver 4\n"
    
    send_message(ip_address, port, message)
