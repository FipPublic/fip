use serde::Deserialize;
use std::path::Path;
use config::{Config, File, ConfigError};
use serde::de::DeserializeOwned;
use std::str;

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
    fn it_works() {
        let config_path = "test_config.toml";
        match load_config::<ServerSettings>(config_path) {
            Ok(setting) => println!("Loaded configs: {:?}", setting),
            Err(e) => eprintln!("Failed to load configs: {}", e),
        }
    }
}

#[derive(Debug, Deserialize)]
pub struct ServerSettings {
    connection_multiple: bool,  // enable connection multiple
    max_multiple: i8,           // max connection on one connect
}

#[derive(Debug, Deserialize)]
pub struct ClientSettings {}