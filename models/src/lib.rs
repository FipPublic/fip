

// Handshake is a data struct that use in build core when client connect to server
pub struct Handshake {
    client_name: str,
    remote_ip: str,
    auth: str,
}