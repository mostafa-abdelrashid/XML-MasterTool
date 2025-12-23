# XML MasterTool - Social Network Analysis & XML Editor

## 📌 Project Overview
**XML MasterTool** is a high-performance C++ command-line application engineered to parse, manipulate, and analyze XML data without relying on external libraries. It serves a dual purpose: acting as a robust **XML Editor** for formatting and compression, and functioning as a **Social Network Analysis Engine** that interprets XML datasets as a directed graph of users and followers.

This project demonstrates advanced implementation of custom data structures (N-ary Trees, Directed Graphs) and algorithms (Byte Pair Encoding, Recursive Descent Parsing) to handle data efficiently.

---

## 🚀 Key Features

### Level 1: XML File Management
Core utilities for maintaining and converting XML data.

* **XML Verification & Error Recovery:**
    * **Algorithm:** Stack-based validation to track opening/closing tags.
    * **Functionality:** Detects structural errors (mismatched tags, missing brackets) and includes an **Auto-fix (`-f`)** mode that intelligently inserts missing closing tags to restore file validity.
* **Formatting (Prettify):**
    * Parses the raw XML and reconstructs it with consistent indentation (4 spaces) and newlines, converting single-line files into human-readable structures.
* **Minification:**
    * Strips all unnecessary whitespace, newlines, and indentation to reduce file size for efficient storage or transmission.
* **JSON Conversion:**
    * Converts the XML DOM tree into a JSON object, preserving hierarchy, attributes, and handling array structures for repeated tags (e.g., lists of posts or followers).
* **Compression (BPE):**
    * **Algorithm:** Implements **Byte Pair Encoding (BPE)**.
    * **Functionality:** Iteratively replaces the most frequent byte pairs with unused byte tokens to significantly reduce text size. Includes a corresponding decompressor to restore the original XML.

### Level 2: Social Network Graph Analysis
The tool parses specific user-profile XML schemas to build an in-memory social graph.

* **Network Metrics:**
    * **Most Active User:** Identifies the user with the highest degree centrality (sum of followers and following connections).
    * **Most Influential User:** Identifies the user with the highest in-degree (number of followers).
* **Graph Algorithms:**
    * **Mutual Followers:** Uses Hash Set intersection (`std::unordered_set`) to find common followers between multiple users with $O(N)$ complexity.
    * **User Suggestions:** Implements a "Friends of Friends" recommendation algorithm to suggest connections based on the network topology.
* **Content Search:**
    * Performs linear search across user posts to find specific **keywords** in the post body or filter by **topics** (e.g., "sports", "economy").
* **Visualization:**
    * Exports the internal graph structure to a `.dot` file and automates the rendering of a visual network map using **Graphviz**.


---

## 🛠️ Installation & Build

### Prerequisites
* **C++ Compiler:** GCC (`g++`) or Clang (supporting C++11 or higher).
* **Graphviz:** Required only for the `draw` command.
    * *Windows:* [Download Installer](https://graphviz.org/download/) (Add to system PATH during installation).
    * *Linux:* `sudo apt-get install graphviz`

### Compilation
To compile the entire project, run the following command in the root directory:

```bash
g++ *.cpp Classes/*.cpp -o xml_editor
```


## ⚙️ Technical Implementation

### Data Structures
