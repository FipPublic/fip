use serde::Deserialize;
use std::path::Path;
use config::{Config, File, ConfigError};
use serde::de::DeserializeOwned;
use std::str;
use std::sync::Mutex;
use lazy_static::lazy_static;

const SERVER_PORT: u16 = 7800;

lazy_static! {
    pub static ref SERVER_CONFIG: Mutex<ServerConfig> = Mutex::new(ServerConfig{
        connection_multiple: true,
        max_multiple: 2,
        server_port: SERVER_PORT,
        tcp_port: 7801,
        kcp_port: 7802
    });
    pub static ref CLIENT_CONFIG: Mutex<ClientConfig> = Mutex::new(ClientConfig {
        server_port: SERVER_PORT
    });
}

pub fn load_config<T>(config_path: &str) -> Result<T, ConfigError>
    where
        T: DeserializeOwned
{
    let builder = Config::builder().add_source(File::from(Path::new(config_path)));
    let settings = builder.build()?;
    settings.try_deserialize()
}


#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn test_load_config() {
        let config_path = "test_config.toml";
        match load_config::<ServerConfig>(config_path) {
            Ok(setting) => println!("Loaded configs: {:?}", setting),
            Err(e) => eprintln!("Failed to load configs: {}", e),
        }
    }

    #[test]
    fn test_change_global_config() {
        let mut sc = SERVER_CONFIG.lock().unwrap();
        sc.kcp_port = 2202;
        if (SERVER_CONFIG.lock().unwrap().kcp_port != 2202) {
            eprintln!("Failed to change global config: {:?}",SERVER_CONFIG.lock().unwrap())
        }
    }
}

#[derive(Debug, Deserialize)]
pub struct ServerConfig {
    connection_multiple: bool,      // enable connection multiple
    max_multiple: u16,              // max connection on one connect
    server_port: u16,               // client connect port
    tcp_port: u16,                  // tpc listen port
    kcp_port: u16,                  // kcp listen port
}

#[derive(Debug, Deserialize)]
pub struct ClientConfig {
    server_port: u16,    // server port
}