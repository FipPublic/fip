use std::error::Error;

// Server is trait that all different server communication protocol need impl
pub trait Server: Proxy {
    fn run_and_listen() -> Result<(), Box<dyn Error>>;
}

// Client is trait that all different client communication protocol need impl
pub trait Client: Proxy {
    fn connect_server() -> Result<(), Box<dyn Error>>;
}

pub trait Proxy {
    fn read_buffer() -> [u8; 512];
    fn write_buffer(buffer: [u8; 512]) -> Result<(), Box<dyn Error>>;
}


fn run_core() {
    let config = configs::SERVER_CONFIG.lock().unwrap();
    

}