# 📚 Shared Library

(Common infrastructure and reusable components shared across the MiniBanking ecosystem.)

---

## 📌 Overview

The Shared module contains reusable infrastructure components that are utilized by both the Application and the Collector.

Instead of duplicating common functionality across multiple projects, shared services are centralized into a dedicated library to improve maintainability, consistency, and code reuse.

The Shared module is not intended to provide business logic. Instead, it supplies foundational services required by higher-level components.

---

## 🎯 Core Responsibilities

The Shared module provides common infrastructure for:

- Database connectivity.
- Database helper utilities.
- File handling.
- System logging.
- Database error handling.
- ODBC utility functions.

These components are designed to remain independent from application-specific business logic.

---

## 🏗️ Architecture Overview

The Shared module serves as the common infrastructure layer of the MiniBanking ecosystem.

It is referenced by both the Banking Application and the Collector, allowing both components to reuse the same implementation for low-level services.

The module consists of five logical components:

- 🛢️ Database Layer
- ⚙️ Utility Layer
- 📝 Logging Layer
- ❗ Error Layer
- 📦 Shared Data Structures

---

## 📐 Project Structure

```text
Shared
│
├── data
│   └── sql_error.h
│
├── database
│   ├── db_connection.cpp
│   └── db_connection.h
│
├── error
│   ├── database_errors.h
│   └── system_errors.h
│
├── logging
│   ├── system_log.cpp
│   └── system_log.h
│
└── utils
    ├── db_helper.
    ├── file_handle.
    └── odbc_helper_error.
```

---

## 🧩 Shared Components

### 🛢️ Database Layer

Responsible for establishing and managing SQL Server connections.

Responsibilities:

- Database connection initialization.
- Connection lifecycle management.
- Shared ODBC configuration.

---

### ⚙️ Utility Layer

Provides reusable helper functions used throughout the project.

Responsibilities:

- Database helper functions.
- File reading and writing.
- ODBC utility functions.
- Common helper routines.

The Utility Layer simplifies repetitive low-level operations while keeping higher-level components focused on business logic.

---

### 📝 Logging Layer

Provides a centralized mechanism for writing system logs.

Responsibilities:

- System log generation.
- Runtime logging.
- Internal diagnostic messages.

Centralizing logging ensures consistent log formatting across different project components.

---

### ❗ Error Layer

Defines common error types shared throughout the project.

Responsibilities:

- Database error definitions.
- System-level error definitions.
- Shared exception information.

Separating common error definitions avoids duplicated error handling logic and improves maintainability.

---

### 📦 Shared Data

Contains lightweight data structures shared between multiple modules.

Current shared data includes:

- SQL error information.

Additional shared structures can be introduced as the project evolves.

---

## 🎯 Design Goals

The Shared module was designed with the following objectives:

- Promote code reuse.
- Reduce duplicated infrastructure code.
- Centralize common services.
- Improve maintainability.
- Decouple infrastructure from business logic.

By separating reusable infrastructure from application-specific functionality, both the Banking Application and the Collector remain easier to maintain and extend independently.
