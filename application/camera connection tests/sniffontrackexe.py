import psutil
import socket
from scapy.layers.inet import IP, UDP
from scapy.all import *

def sniff_tcp_connections_and_store_data():
    # Open a file to store the received data
    with open("tracking_data.txt", "w") as file:
        # Get all running processes
        for process in psutil.process_iter(['pid', 'name']):
            try:
                if process.info['name'] == "Track.exe":
                    # Get the TCP connections for the "Track.exe" process
                    connections = psutil.Process(process.info['pid']).connections('tcp')
                    for conn in connections:
                        if conn.status == psutil.CONN_ESTABLISHED:
                            # Create a socket to receive data
                            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                                s.bind((conn.laddr.ip, conn.laddr.port))
                                s.listen(1)
                                conn, _ = s.accept()
                                # Receive data from the connection
                                data = conn.recv(1024)
                                # Write the received data to the file
                                file.write(data.decode('utf-8'))
                                file.write("\n")  # Add a newline for better readability
                                # Print the received data
                                print("Received data:", data.decode('utf-8'))
            except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
                pass

def sniff_tcp(HOST, PORT):
    # Create a TCP/IP socket
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # Bind the socket to the address and port
        s.bind((HOST, PORT))
        # Get the dynamically assigned port
        _, port = s.getsockname()
        print(f"Listening on all interfaces on port {port}...")

        # Listen for incoming connections
        s.listen()

        # Accept incoming connections
        conn, addr = s.accept()
        with conn:
            print('Connected by', addr)

            # Open a file to write the data
            with open('sniffed_data.txt', 'a') as file:
                while True:
                    # Receive data from the connection
                    data = conn.recv(1024)
                    if not data:
                        break
                    # Write the received data to the file
                    file.write(data.decode('utf-8'))
                    file.flush()

            print('Connection closed')

def packet_handler(packet):
    if IP in packet and UDP in packet:
        print("set IP and etc")
        src_ip = packet[IP].src
        src_port = packet[UDP].sport
        dst_ip = packet[IP].dst
        dst_port = packet[UDP].dport
        data = packet[Raw].load.decode('utf-8') if Raw in packet else ""
        
        with open('sniffed_udp_data.txt', 'a') as file:
            file.write(f"Source IP: {src_ip}, Source Port: {src_port} --> Destination IP: {dst_ip}, Destination Port: {dst_port}, Data: {data}\n")
            file.flush()

def main():
    #print("Sniffing established TCP connections and storing data in tracking_data.txt...")
    #sniff_tcp_connections_and_store_data()
    # Define the local address and port to listen on
    #HOST = '131.246.110.186'  # Change this to the IP address you want to listen on
    #PORT = 0 # Change this to the port you want to listen on
    #sniff_tcp(HOST,PORT)
    # Sniff UDP packets
    print("Start sniffing")
    sniff(filter="udp", prn=packet_handler, store=0)
    print("Data stored successfully.")

if __name__ == "__main__":
    main()
