use clap::{Command, Arg};
use std;
use std::string::String;
use configs::{CLIENT_CONFIG, SERVER_CONFIG};


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
        Some(("install",input)) => {
            // todo install service to system
        }
        Some(("run",input)) => {
            println!("{:?}", SERVER_CONFIG.lock().unwrap());
            println!("{:?}", CLIENT_CONFIG.lock().unwrap());
            if let Some(config) = input.get_one::<String>("config") {
                // todo read configuration file and init
            }
            // todo start run server
        }
        _ => {
            println!("Run '{} --help' for more information on a command.", NAME);
        },
    }
}
