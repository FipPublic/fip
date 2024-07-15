use clap::{Command, Arg};
use std;
use std::string::String;
use configs::{SERVER_CONFIG, load_config, ServerConfig};


const NAME: &str = "fip_server";
const VERSION: &str = "0.0.1";
const AUTHOR: &str = "FIP";
const ABOUT: &str = "a high-performance Intranet penetration service";

fn main() {
    let cmd = Command::new(NAME)
        .bin_name(NAME)
        .author(AUTHOR)
        .version(VERSION)
        .about(ABOUT)
        .subcommands([
            Command::new("install").about("install fip server as system service"),
            Command::new("run").about("run fip server").arg(
                Arg::new("config").short('c').long("config").help("fip configuration toml files")
            ),
        ])
        .get_matches();

    match cmd.subcommand() {
        Some(("install", input)) => {
            // todo install service to system
        }
        Some(("run", input)) => {
            if let Some(config) = input.get_one::<String>("config") {
                match load_config::<ServerConfig>(config) {
                    Ok(serverConfig) => {
                        let mut server_config = SERVER_CONFIG.lock().unwrap();
                        server_config = server_config;
                    }
                    Err(e) => {
                        panic!("Server config parse error {e}");
                    }
                }
            }
            // todo start run server
        }
        _ => {
            panic!("Run '{} --help' for more information on a command.", NAME);
        }
    }
}
