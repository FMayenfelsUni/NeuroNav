import psutil
import socket

def sniff_tcp_connections():
    # Get all running processes
    for process in psutil.process_iter(['pid', 'name']):
        try:
            # Get the TCP connections for each process
            connections = psutil.Process(process.info['pid']).connections('tcp')
            for conn in connections:
                if conn.status == psutil.CONN_ESTABLISHED:
                    print(f"Process ID: {process.info['pid']}, Process Name: {process.info['name']}")
                    print(f"Local Address: {conn.laddr.ip}:{conn.laddr.port} --> Remote Address: {conn.raddr.ip}:{conn.raddr.port}")
                    print("-----------------------------------------------------------------")
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            pass

def main():
    print("Sniffing established TCP connections:")
    sniff_tcp_connections()

if __name__ == "__main__":
    main()
