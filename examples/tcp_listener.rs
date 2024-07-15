use tokio::net::{TcpListener, TcpStream};
use tokio::io::{AsyncReadExt, AsyncWriteExt};

async fn handle_client(mut stream: TcpStream) {
    let mut buffer = [0; 512];
    loop {
        match stream.read(&mut buffer).await {
            Ok(0) => break,
            Ok(n) => {
                println!("Received: {}", String::from_utf8_lossy(&buffer[..n]));
                if let Err(e) = stream.write_all(&buffer[..n]).await {
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

#[tokio::main]
async fn main() -> std::io::Result<()> {

    let listener = TcpListener::bind("127.0.0.1:7878").await?;
    println!("Server listening on port 7878");

    loop {
        match listener.accept().await {
            Ok((stream, _)) => {
                tokio::spawn(async move {
                    handle_client(stream).await;
                });
            }
            Err(e) => {
                println!("Failed to accept a core: {}", e);
            }
        }
    }
}
