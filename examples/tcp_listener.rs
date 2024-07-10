use std::io::{Read, Write};
use std::net::{TcpListener, TcpStream};
use std::thread;
fn main() -> std::io::Result<()> {
    let listener = TcpListener::bind("0.0.0.0:8080")?;
    println!("Server listening on port 7878");

    for stream in listener.incoming() {
        match stream {
            Ok(stream) => {
                thread::spawn(|| {
                    handle_client(stream);
                });
            }
            Err(e) => {
                println!("Failed to accept a connection: {}", e);
            }
        }
    }
    Ok(())
}

fn handle_client(mut stream: TcpStream) {
    let mut buffer = [0; 512];
    loop {
        match stream.read(&mut buffer) {
            Ok(0) => break,
            Ok(n) => {
                println!("Received: {}", String::from_utf8_lossy(&buffer[..n]));
                if let Err(e) = stream.write(&buffer[..n]) {
                    println!("Failed to send response: {}", e);
                    break;
                }
            }
            Err(e) => {
                println!("Failed to read from socket: {}", e);
                break;
            }
        }
    }
}
