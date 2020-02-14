import threading
import socket
import selectors

tcp_selector = selectors.DefaultSelector()
client_socket_list = []


def data_read_client(client_socket, mask):
    try:
        data = client_socket.recv(1024)
        if data:
            print(data.decode())
        else:
            tcp_selector.unregister(client_socket)
            client_socket.close()
            client_socket_list.remove(client_socket)
    except:
        tcp_selector.unregister(client_socket)
        client_socket.close()
        client_socket_list.remove(client_socket)


def create_tcp_link(server_socket, addr):
    client_socket, addr = server_socket.accept()
    client_socket_list.append(client_socket)
    client_socket.setblocking(False)
    tcp_selector.register(client_socket, selectors.EVENT_READ, data_read_client)


def create_tcp_listen(host, port):
    tcp_server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    tcp_server_sock.bind((host, port))
    tcp_server_sock.listen()
    tcp_server_sock.setblocking(False)
    tcp_selector.register(tcp_server_sock, selectors.EVENT_READ, create_tcp_link)
    while True:
        events = tcp_selector.select()
        for key, mask in events:
            callback = key.data
            callback(key.fileobj, mask)
    # tcp_server_sock.close()
