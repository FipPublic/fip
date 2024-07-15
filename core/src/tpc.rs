use std::error::Error;
use std::future::Future;
use tokio::net::{TcpListener as Tcp, TcpStream};
use configs::{SERVER_CONFIG};
use crate::core::{Proxy, Server};

struct TcpListener {

}

pub fn init_tcp_listener() {

}

impl Server for TcpListener {

    async fn run_and_listen() -> Result<(), Box<dyn Error>> {

        let server_config = SERVER_CONFIG.lock().unwrap();
        let mut listen_address = String::new();
        listen_address.push_str("0.0.0.0:");
        listen_address.push_str(server_config.tcp_port.to_string().as_str());

        let tcp_listener = Tcp::bind(listen_address)?;

    }
}

impl Proxy for TcpListener {
    fn read_buffer() -> [u8; 512] {
        todo!()
    }

    fn write_buffer(buffer: [u8; 512]) -> Result<(), Box<dyn Error>> {
        todo!()
    }
}
