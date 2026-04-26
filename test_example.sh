# Add a report (both roles)
./city_manager --role manager --user alice --add downtown
./city_manager --role inspector --user bob --add downtown

# List reports in a district
./city_manager --role manager --user alice --list downtown

# View a specific report
./city_manager --role manager --user alice --view downtown 1475592310

# Remove a report (manager only)
./city_manager --role manager --user alice --remove_report downtown 1475592310

# Update severity threshold (manager only)
./city_manager --role manager --user alice --update_threshold downtown 3

# Filter reports by condition
./city_manager --role inspector --user bob --filter downtown severity:>=:2
./city_manager --role inspector --user bob --filter downtown severity:>=:2 category:==:road

# Help
./city_manager --help