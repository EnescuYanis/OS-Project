# City Infrastructure Reporting System (Phase 1)

A C-based UNIX system implementing a city infrastructure issue reporting and monitoring system. This project demonstrates low-level system programming, binary file I/O, manual permission enforcement, and AI-integrated filtering.

## 📁 Project Structure

The system organizes data by district directories. Each district contains:
- `reports.dat`: A binary file storing fixed-size report records.
- `district.cfg`: A plain-text configuration file for severity thresholds.
- `logged_district`: An operation log for auditing actions.

## 🛠️ Requirements & System Calls

This project is built using standard UNIX system calls:
- **File Management:** `open`, `close`, `read`, `write`, `lseek`, `ftruncate`
- **Metadata & Permissions:** `stat`, `lstat`, `chmod`, `mkdir`
- **Links:** `symlink`, `unlink`

## 🔐 Role-Based Access Control (Simulation)

The system recognizes two roles: `manager` and `inspector`. Access is enforced by checking UNIX permission bits before execution.

| File/Resource | Permissions | Manager Access | Inspector Access |
|:--- |:--- |:--- |:--- |
| District Directory | `750` | rwx (Owner) | r-x (Group) |
| `reports.dat` | `664` | rw- | rw- |
| `district.cfg` | `640` | rw- | r-- |
| `logged_district` | `644` | rw- | r-- |

## 🚀 Usage

### Compilation
```bash
gcc -Wall -Wextra -o city_manager city_manager.c