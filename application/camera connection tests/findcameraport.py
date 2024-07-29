import socket

def scan_ports(target_ip, start_port, end_port):
    print(f"Scanning ports {start_port} to {end_port} on {target_ip}...")
    open_ports = []
    for port in range(start_port, end_port + 1):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.settimeout(0.5)  # Set a timeout for the connection attempt
            result = s.connect_ex((target_ip, port))
            if result == 0:
                print(f"Port {port} is open")
                open_ports.append(port)
    return open_ports

def main():
    target_ip = input("Enter the target IP address: ")
    start_port = int(input("Enter the starting port: "))
    end_port = int(input("Enter the ending port: "))
    open_ports = scan_ports(target_ip, start_port, end_port)
    if open_ports:
        print("Open ports found:", open_ports)
    else:
        print("No open ports found.")

if __name__ == "__main__":
    main()
