# Road Map & Route Finder

A C++ geographic navigation application that implements **Dijkstra's shortest-path algorithm** to calculate routes between locations across a large road network in the United States.

The application combines graph-based route calculation, hash table searching, geographic data processing, interactive road exploration, and terrain visualization to generate detailed driving directions and graphical route maps.

Developed as part of **ECE 318: Algorithms** at the University of Miami.

## Features

- **Shortest-Path Routing:** Implements Dijkstra's algorithm to determine the shortest available route between two locations based on road distance.
- **Geographic Place Search:** Uses a custom hash table with linked-list collision handling to retrieve locations by name.
- **Graph Representation:** Represents intersections as vertices and road connections as weighted edges using adjacency lists.
- **Interactive Road Exploration:** Allows users to navigate connected roads and examine individual intersections.
- **State Selection:** Distinguishes between locations with identical names in different states.
- **Detailed Route Directions:** Displays road names, travel distances, compass directions, geographic coordinates, and nearby locations.
- **Terrain Visualization:** Processes geographic elevation data to generate terrain maps with highlighted routes.
- **Bitmap Image Generation:** Exports generated route maps as BMP image files.
- **Input Validation:** Handles invalid menu selections, missing data files, and invalid road records.

## Technologies Used

| Technology | Application |
|---|---|
| C++ | Core application development |
| Microsoft Visual Studio | Development and debugging |
| Dijkstra's Algorithm | Shortest-path calculation |
| Hash Tables | Geographic place searching |
| Linked Lists | Collision handling and graph connections |
| Adjacency Lists | Road network representation |
| File I/O | Geographic dataset processing |
| BMP Graphics Library | Terrain and route visualization |

## How It Works

### 1. Geographic Data Processing

The application loads geographic information from external data files containing named places, state abbreviations, road intersections, road connections, and elevation coverage.

Each named place contains geographic information such as:

- Place name and identification number
- State
- Population
- Geographic area
- Latitude and longitude
- Associated road intersection
- Distance to the associated intersection

### 2. Hash Table Implementation

A custom hash table containing 1,009 buckets stores geographic place records.

Place names are processed through a polynomial hash function, and collisions are handled using separate chaining with linked lists.

When multiple locations share the same name, the application displays matching states and allows the user to select the intended destination.

### 3. Road Network Construction

The application constructs a weighted, undirected graph representing the road network.

- **Vertices:** Road intersections
- **Edges:** Road connections
- **Weights:** Road segment lengths in miles

Each intersection maintains an adjacency list of connected roads, allowing the application to explore neighboring intersections and calculate routes.

### 4. Shortest-Path Calculation

Dijkstra's algorithm determines the shortest route between the road intersections associated with the selected starting and destination locations.

The application reconstructs the calculated route and displays information for each road segment, including:

- Intersection identification number
- Latitude and longitude
- Nearby geographic location
- Road name
- Road classification code
- Segment distance
- Compass direction

### 5. Terrain Map Generation

The application processes geographic elevation data to generate a terrain-based visualization of the calculated route.

The resulting map is exported as `map.bmp`, allowing users to view the route geographically rather than relying exclusively on text-based directions.

## Application Interface

The program provides an interactive command-line menu:

```text
Welcome to the Road Map!

Press 1 to explore roads
Press 2 to find a shortest route
Press 0 to quit
Choice:
```

### Road Exploration

Allows users to select a geographic location, inspect its associated intersection, and navigate through connected roads.

### Shortest Route Finder

Allows users to select starting and destination locations, calculate the shortest available route, display detailed directions, and generate a graphical route map.

## Generated Route Maps

The following route maps were generated during project testing:

| Starting Location | Destination | Generated Map |
|---|---|---|
| Chicago, Illinois | Portland, Oregon | [View Map](outputs/Chicago-Portland.bmp) |
| Dallas, Texas | Fort Worth, Texas | [View Map](outputs/Dallas-FortWorth.bmp) |
| Denver, Colorado | Detroit, Michigan | [View Map](outputs/Denver-Detroit.bmp) |
| Houston, Texas | Orlando, Florida | [View Map](outputs/Houston-Orlando.bmp) |
| Los Angeles, California | Salem, Oregon | [View Map](outputs/LosAngeles-Salem.bmp) |
| Phoenix, Arizona | San Diego, California | [View Map](outputs/Phoenix-SanDiego.bmp) |

These examples demonstrate both local and long-distance route calculations across different geographic regions.

## Project Structure

```text
Road-Map-and-Route-Finder/
├── README.md
├── .gitignore
├── main.cpp
├── makebmp.cpp
├── makebmp.h
│
├── data/
│   ├── connections.txt
│   ├── intersections.txt
│   ├── named-places.txt
│   ├── states.txt
│   ├── coverage.txt
│   ├── Digital Elevation Models
│
└── outputs/
    ├── Chicago-Portland.bmp
    ├── Dallas-FortWorth.bmp
    ├── Denver-Detroit.bmp
    ├── Houston-Orlando.bmp
    ├── LosAngeles-Salem.bmp
    └── Phoenix-SanDiego.bmp
```

## Geographic Datasets

| File | Description |
|---|---|
| `named-places.txt` | Geographic locations and associated place information |
| `states.txt` | State abbreviations and full state names |
| `intersections.txt` | Road intersection coordinates and geographic information |
| `connections.txt` | Road connections, endpoints, classifications, and distances |
| `coverage.txt` | Geographic elevation coverage information |
| Elevation archives | 149 elevation data files used for terrain visualization |

The elevation dataset is divided into three ZIP archives to accommodate repository upload size limitations while preserving the original geographic data.

## Compilation and Execution

### Requirements

- Microsoft Visual Studio with C++ support or a compatible C++ compiler
- Required geographic datasets
- BMP graphics utility files
- Extracted geographic elevation data

### Setup Instructions

1. Download or clone the repository.
2. Create a C++ console application in Microsoft Visual Studio.
3. Add `main.cpp`, `makebmp.cpp`, and `makebmp.h` to the project.
4. Extract all three elevation data archives.
5. Place the required geographic data files in the application's working directory.
6. Build and run the application.
7. Select road exploration or shortest-route calculation from the main menu.

Generated route maps are saved as `map.bmp` in the application's working directory.

## Testing

The application was exercised using multiple geographic locations and road routes.

Testing focused on:

- Geographic place searching
- Locations with identical names in different states
- Interactive road navigation
- Short-distance route calculation
- Long-distance route calculation
- Road distance and direction reporting
- Terrain data processing
- Bitmap map generation

Generated route maps are available in the [`outputs/`](outputs/) directory.

## Project Limitations

- Route calculations depend on the accuracy and coverage of the supplied geographic datasets.
- Routes are optimized for road distance rather than real-time traffic or estimated travel time.
- Terrain visualization requires the corresponding elevation data files.
- The application uses a command-line interface rather than an interactive graphical navigation interface.

## Acknowledgments

The geographic datasets and BMP graphics utilities were provided as supporting materials for university coursework.

The project implementation focuses on geographic data processing, custom data structures, road network construction, shortest-path algorithms, and route visualization.

## Project Background

Developed as part of **ECE 318: Algorithms** at the University of Miami.

This project provided practical experience applying graph theory, algorithm design, hashing, linked data structures, geographic data processing, and file management to a real-world navigation problem.
