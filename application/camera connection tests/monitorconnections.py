import psutil
import socket

def list_connections(ip_address):
    connections = psutil.net_connections(kind='inet')
    for conn in connections:
        if conn.raddr and conn.raddr.ip == ip_address:
            local_address = ":".join([conn.laddr.ip, str(conn.laddr.port)])
            remote_address = ":".join([conn.raddr.ip, str(conn.raddr.port)])
            print(f"Local Address: {local_address} -> Remote Address: {remote_address}")
            print(f"Status: {conn.status}")
            print(f"PID: {conn.pid}")
            if conn.pid:
                try:
                    process = psutil.Process(conn.pid)
                    print(f"Process Name: {process.name()}")
                    print(f"Process Command Line: {process.cmdline()}")
                except psutil.NoSuchProcess:
                    print("Process information not available.")
            print("----------------------------------------")

def main():
    ip_address = input("Enter the IP address to monitor connections: ")
    list_connections(ip_address)

if __name__ == "__main__":
    main()
