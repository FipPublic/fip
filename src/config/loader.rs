use std::env;
use config::{Config, File};
use crate::config::settings::Settings;

pub fn load_config() -> Result<Settings, config::ConfigError> {
    let env = env::var("RUN_MODE").unwrap_or_else(|_| "development".into());
    let mut settings = Config::new();

    settings.merge(File::with_name(&format!("config/{}", env)).required(true))?;
    settings.try_into()
}
