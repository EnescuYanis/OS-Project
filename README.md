# City Infrastructure Issue Reporting and Monitoring System
## Project Intention & Phase 1 Architecture Overview

### 1. Executive Summary
The **City Infrastructure Issue Reporting and Monitoring System** is a robust, UNIX-based application written in C, designed to streamline the reporting, storage, and management of urban infrastructure problems (such as damaged roads, broken lighting, and flooding). 

The primary intention of the project is to simulate a real-world systems programming environment where city inspectors can securely file structured reports, and managers can administer them. The project serves as an extensive practical application of UNIX system programming concepts, heavily emphasizing programmatic access control, binary file manipulation, and file system metadata management.

---

### 2. Phase 1 Objectives: The File System Foundation
Phase 1 establishes the core storage, querying, and access management layers of the application. The primary objectives are to:
* **Master System Calls:** Transition from standard C library I/O (like `fopen`, `fread`) to direct UNIX system calls (`open()`, `read()`, `write()`, `lseek()`, `stat()`, `chmod()`).
* **Enforce Role-Based Access Control (RBAC):** Implement strict security policies programmatically based on user roles and UNIX file permission bits.
* **Manage Binary Data:** Efficiently pack, read, and manipulate fixed-size C structures (records) directly on disk without gaps, including in-place record deletion.
* **Integrate AI Tools:** Responsibly utilize AI-assisted code generation for specific string-parsing tasks, accompanied by critical evaluation and documentation.

---

### 3. System Architecture & Role Management

To simulate a multi-user environment on shared academic machines, the system employs command-line argument passing to declare active roles and user identities dynamically:
* **Manager (`--role manager`):** Acts as the "owner" of the files, possessing administrative privileges to remove reports, modify configuration thresholds, and perform all read/write operations.
* **Inspector (`--role inspector`):** Acts as a member of the owning "group," possessing restricted permissions primarily focused on adding new reports and reading existing data.

---

### 4. Data Layout & Persistence
The system uses a highly structured, decentralized approach to data storage, creating a dedicated ecosystem for each city district.

#### Directory Structure
For every district, the system generates a directory containing three critical files:
1. `reports.dat`: A binary file storing contiguous, fixed-size records of infrastructure issues.
2. `district.cfg`: A plain-text configuration file storing operational thresholds (e.g., minimum severity level for escalations).
3. `logged_district`: An audit trail logging every action performed within the district, capturing the timestamp, role, and username.

#### The Binary Record Structure
Each report is an atomic, fixed-size block containing:
* **Report ID** (Integer)
* **Inspector Name** (Fixed-length string)
* **GPS Coordinates** (Latitude and longitude floats)
* **Issue Category** (Fixed-length string)
* **Severity Level** (Integer: 1 to 3)
* **Timestamp** (UNIX `time_t`)
* **Description** (Fixed-length string)

---

### 5. Security & Permission Enforcement
A core intention of this system is to prevent unauthorized access at the system call level. Before executing any command, the application must extract permission bits via `stat()` or `lstat()` and verify them against a strict, hardcoded security matrix:

| Asset | Required Permissions | Manager Access | Inspector Access |
| :--- | :--- | :--- | :--- |
| **District Directory** | `750` (`rwxr-x---`) | Full (Read/Write/Execute) | Read/Execute Only |
| `reports.dat` | `664` (`rw-rw-r--`) | Read/Append | Read/Append |
| `district.cfg` | `640` (`rw-r-----`) | Read/Write | Read Only |
| `logged_district`| `644` (`rw-r--r--`) | Read/Write | Read Only |

*Any attempt to bypass these restrictions results in the application safely aborting the operation with a diagnostic access denial.*

---

### 6. Core Operational Capabilities
The `city_manager` executable supports a suite of essential commands:
* **`--add`**: Safely appends binary records to the district's `.dat` file.
* **`--list`**: Iterates through binary records and performs bitwise operations to dynamically translate and print raw `st_mode` permissions into human-readable symbolic strings (e.g., `rw-rw-r--`).
* **`--view`**: Queries and formats a specific record's details.
* **`--remove_report`**: A complex, manager-only operation that utilizes `lseek()` to physically shift trailing binary records backwards to overwrite a deleted entry, followed by `ftruncate()` to shrink the file size, ensuring zero wasted disk space.
* **`--update_threshold`**: Modifies the `district.cfg` file, rigorously verifying that the file's `640` permissions have not been externally tampered with prior to writing.
* **`--filter`**: A powerful query engine allowing users to search reports using dynamic, chained conditions (e.g., `severity:>=:2`).

---

### 7. File System Topology
To facilitate quick access, the system utilizes symbolic links. The application dynamically creates and maintains links named `active_reports-<district_id>` in the root execution folder, pointing directly to deep-nested binary report files. The system must gracefully handle and report "dangling" symlinks using `lstat()`.


### 8. Excectuion and Usage
Run the `city_manager` executable or use the `test_example` to benchtest it before using it.

---

*Document generated based on Phase 1 System Specifications.*