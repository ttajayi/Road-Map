#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include "makebmp.h"
using namespace std;






int string_to_int(string s)
{
    int number = 0;
    for (int n = 0; n < (int)s.length(); n+=1)
    {
        if (s[n] >= '0' && s[n] <= '9')
        {
            number = number * 10 + (s[n] - '0');
        }
    }


    return number;
}


double string_to_double(string s)
{
    double number = 0.0;
    double decimal_place = 0.1;
    bool decimal_found = false;
    bool negative = false;
    for (int n = 0; n < (int)s.length(); n+=1)
    {
        if (s[n] == '-')
        {
            negative = true;
        }
        else if (s[n] == '.')
        {
            decimal_found = true;
        }
        else if (s[n] >= '0' && s[n] <= '9')
        {
            int digit = s[n] - '0';
            if (decimal_found == false)
            {
                number = number * 10 + digit;
            }
            else
            {
                number = number + digit * decimal_place;
                decimal_place = decimal_place / 10;
            }
        }
    }
    if (negative == true)
    {
        number = -number;
    }
    return number;
}




string trim_right(string s)
{
    while (s.length() > 0 && (s[s.length() - 1] == ' ' || s[s.length() - 1] == '\r'))
    {
        s.erase(s.length() - 1, 1);
    }
    return s;
}




class Place
{
public:
    int id;
    string state;
    string name;
    int population;
    double area;
    double latitude;
    double longitude;
    int road_ID;
    double road_distance;
};




class Node
{
public:
    Place data;
    Node * next;
    Node(Place p)
    {
        data = p;
        next = NULL;
    }
};




class hash_table
{
private:
    static const int table_size = 1009;
    Node * table[table_size];
public:
    hash_table()
    {
        for (int n = 0; n < table_size; n+=1)
        {
            table[n] = NULL;
        }
    }


    int hash_function(string name)
    {
        unsigned int hash = 0;
        for (int n = 0; n < (int)name.length(); n+=1)
        {
            hash = hash * 31 + name[n];
        }
        return hash % table_size;
    }


    void insert(Place p)
    {
        int index = hash_function(p.name);
        Node * new_node = new Node(p);
        new_node->next = table[index];
        table[index] = new_node;
    }
    Node * search(string name)
    {
        int index = hash_function(name);
        return table[index];
    }


    ~hash_table()
    {
        for (int n = 0; n < table_size; n+=1)
        {
            while (table[n] != NULL)
            {
                Node * old = table[n];
                table[n] = old->next;
                delete old;
            }
        }
    }
};


class State
{
public:
    string abbreviation;
    string full_name;
};


State states[100];
int state_count = 0;
hash_table places;


string state_name(string abbreviation)
{
    for (int n = 0; n < state_count; n+=1)
    {
        if (states[n].abbreviation == abbreviation)
        {
            return states[n].full_name;
        }
    }
    return abbreviation;
}




bool read_places()
{
    ifstream input("named-places.txt");
    string line;
    if (!input)
    {
        cout << "Couldn't open named-places.txt" << endl;
        return false;
    }
    while (getline(input, line))
    {
        if (line.length() < 114)
        {
            cout << "Incomplete place record :(" << endl;
            return false;
        }


        Place p;
        p.id = string_to_int(line.substr(0, 8));
        p.state = line.substr(8, 2);
        p.name = trim_right(line.substr(10, 49));
                p.population = string_to_int(line.substr(59, 8));
        p.area = string_to_double(line.substr(67, 13));
        p.latitude = string_to_double(line.substr(80, 10));
        p.longitude = string_to_double(line.substr(90, 11));
        p.road_ID = string_to_int(line.substr(101, 5));
        p.road_distance = string_to_double(line.substr(106, 8));
        places.insert(p);
    }


    return input.eof();
}




bool read_states()
{
    ifstream input("states.txt");
    string line;
    if (!input)
    {
        cout << "Couldn't open states.txt" << endl;
        return false;
    }
    while (getline(input, line))
    {
        if (line.length() >= 3)
        {
            if (state_count == 100)
            {
                return false;
            }
            states[state_count].abbreviation = line.substr(0, 2);
            states[state_count].full_name = trim_right(line.substr(3));
            state_count+=1;
        }
    }
    return input.eof();
}




int read_choice(int maximum)
{
    int choice;
    while (true)
    {
        cin >> choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again: ";
        }
        else if (choice >= 0 && choice <= maximum)
        {
                        cin.ignore(10000, '\n');
            return choice;
        }
        else
        {
            cout << "Enter a number from 0 to " << maximum << ": ";
        }
    }
}




Place * choose_place()
{
    string search_name;
    while (true)
    {
        cout << "Enter a place name (press q to cancel): ";
        if (!getline(cin, search_name) || search_name == "q")
        {
            return NULL;
        }
        Node * current = places.search(search_name);
        Node * temp = current;
        int match_count = 0;
        while (temp != NULL)
        {
            if (temp->data.name == search_name)
            {
                match_count+=1;
                cout << match_count << ". " << state_name(temp->data.state);
                cout << " (place ID " << temp->data.id << ")" << endl;
            }
            temp = temp->next;
        }
        if (match_count == 0)
        {
            cout << "Place not found" << endl;
            continue;
        }
        cout << "Select a place (press 0 to cancel): ";
        int choice = read_choice(match_count);
        if (choice == 0)
        {
            return NULL;
        }
        temp = current;
        int current_match = 0;
        while (temp != NULL)
        {
            if (temp->data.name == search_name)
            {
                current_match+=1;
                if (current_match == choice)
                {
                    return &temp->data;
                }
            }
            temp = temp->next;
        }
    }
}




class Connection;


class RoadLink
{
public:
    Connection * road;
    RoadLink * next;
};


class Intersection
{
public:
    int id;
    double longitude;
    double latitude;
    double distance;
    string state;
    string name;
    RoadLink * roads;
};


class Connection
{
public:
    string name;
    string code;
    double length;
    Intersection * first;
    Intersection * second;
};


const int max_intersections = 30000;
const int max_connections = 50000;
Intersection intersections[max_intersections];
Connection connections[max_connections];
RoadLink road_links[max_connections * 2];
int intersection_count = 0;
int connection_count = 0;




bool read_graph()
{
    ifstream input("intersections.txt");
    if (!input)
    {
        cout << "Couldn't open intersections.txt" << endl;
        return false;
    }
    while (input >> ws && input.peek() != -1)
    {
        if (intersection_count == max_intersections)
        {
            cout << "Too many intersections" << endl;
            return false;
        }


        Intersection & point = intersections[intersection_count];
        point.id = intersection_count;
        point.roads = NULL;
        if (!(input >> point.longitude >> point.latitude >> point.distance >> point.state))
        {
            cout << "Invalid intersection record" << endl;
            return false;
        }
        getline(input >> ws, point.name);
        point.name = trim_right(point.name);
        intersection_count+=1;
    }
    if (!input.eof() || intersection_count == 0)
    {
        return false;
    }
    input.close();
    input.open("connections.txt");
    if (!input)
    {
        cout << "Couldn't open connections.txt" << endl;
        return false;
    }
    while (input >> ws && input.peek() != -1)
    {
        if (connection_count == max_connections)
        {
            cout << "Too many connections" << endl;
            return false;
        }
        Connection & road = connections[connection_count];
        int first;
        int second;
        if (!(input >> road.name >> road.code >> first >> second >> road.length))
        {
            cout << "Invalid connection record" << endl;
            return false;
        }
        if (first < 0 || first >= intersection_count || second < 0 ||
            second >= intersection_count || road.length < 0)
        {
            cout << "Invalid road endpoints or length" << endl;
            return false;
        }
        road.first = &intersections[first];
        road.second = &intersections[second];
        RoadLink * link = &road_links[connection_count * 2];
        link->road = &road;
        link->next = road.first->roads;
        road.first->roads = link;
        link = &road_links[connection_count * 2 + 1];
        link->road = &road;
        link->next = road.second->roads;
        road.second->roads = link;
        connection_count+=1;
    }
    return input.eof();
}




Intersection * other_end(Connection * road, Intersection * current)
{
    if (road->first == current)
    {
        return road->second;
    }
    return road->first;
}




string direction(Intersection * first, Intersection * second)
{
    double pi = acos(-1.0);
    double latitude_difference = (second->latitude - first->latitude) * pi / 180;
    double longitude_difference = (second->longitude - first->longitude) * pi / 180;
    double angle = atan2(longitude_difference, latitude_difference) * 180 / pi;
    if (angle >= -22.5 && angle < 22.5)
        return "N";
    else if (angle >= 22.5 && angle < 67.5)
        return "NE";
    else if (angle >= 67.5 && angle < 112.5)
        return "E";
    else if (angle >= 112.5 && angle < 157.5)
        return "SE";
    else if (angle >= 157.5 || angle < -157.5)
        return "S";
    else if (angle < -112.5)
        return "SW";
    else if (angle < -67.5)
        return "W";
    else
        return "NW";
}




void display_intersection(Intersection * point)
{
    cout << "Intersection " << point->id << ":" << endl;
    cout << fixed << setprecision(4);
    cout << "Latitude: " << point->latitude << endl;
    cout << "Longitude: " << point->longitude << endl;
    cout << "Nearby place: " << point->name << ", " << point->state << endl;
    cout << setprecision(2);
    cout << "Distance to nearby place: " << point->distance << " miles" << endl;
}




void display_road(Connection * road, Intersection * current)
{
    if (road->name == "?")
    {
        cout << "Unnamed road";
    }
    else
    {
        cout << road->name;
    }
    cout << " [" << road->code << "], ";
    cout << fixed << setprecision(3) << road->length << " miles, ";
    cout << direction(current, other_end(road, current));
}




Intersection * choose_intersection()
{
    Place * place = choose_place();
    if (place == NULL)
    {
        return NULL;
    }
    if (place->road_ID <= 0 || place->road_ID >= intersection_count)
    {
        cout << "No intersection found for this place" << endl;
        return NULL;
    }
    return &intersections[place->road_ID];
}




void explore(Intersection * current)
{
    while (true)
    {
        cout << endl;
        display_intersection(current);
        RoadLink * temp = current->roads;
        int count = 0;
        while (temp != NULL)
        {
            count+=1;
            cout << count << ". ";
            display_road(temp->road, current);
            Intersection * next = other_end(temp->road, current);
            cout << " -> intersection " << next->id;
            cout << ", " << next->name << ", " << next->state;
            cout << " (" << setprecision(2) << next->distance << " miles from place)" << endl;
            temp = temp->next;
        }
        cout << "Choose a road (press 0 to return to menu): ";
        int choice = read_choice(count);
        if (choice == 0)
        {
            return;
        }
        temp = current->roads;
        for (int n = 1; n < choice; n+=1)
        {
            temp = temp->next;
        }
        current = other_end(temp->road, current);
    }
}


class DEM
{
public:
    double top;
    double bottom;
    double left;
    double right;
    string filename;
};




bool find_dem(Intersection * start, Intersection * destination, DEM & best)
{
    ifstream input("coverage.txt");
    if (!input)
    {
        cout << "Couldn't open coverage.txt" << endl;
        return false;
    }
    DEM current;
    double smallest = -1;
    while (input >> current.top >> current.bottom >> current.left
                 >> current.right >> current.filename)
    {
        bool start_inside = start->latitude <= current.top &&
                            start->latitude >= current.bottom &&
                            start->longitude >= current.left &&
                            start->longitude <= current.right;
        bool end_inside = destination->latitude <= current.top &&
                          destination->latitude >= current.bottom &&
                          destination->longitude >= current.left &&
                          destination->longitude <= current.right;
        double area = (current.top - current.bottom) *
                      (current.right - current.left);
        if (start_inside && end_inside && (smallest < 0 || area < smallest))
        {
            smallest = area;
            best = current;
        }
    }
    return smallest >= 0;
}




int map_x(double longitude, DEM & dem, int width)
{
    return (int)((longitude - dem.left) * (width - 1) / (dem.right - dem.left) + 0.5);
}




int map_y(double latitude, DEM & dem, int height)
{
    return (int)((dem.top - latitude) * (height - 1) / (dem.top - dem.bottom) + 0.5);
}




void draw_line(image & picture, int x1, int y1, int x2, int y2, int c)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx);


    if (abs(dy) > steps)
        steps = abs(dy);


    if (steps == 0)
        steps = 1;


    for (int n = 0; n <= steps; n+=1)
    {
        int x = x1 + (int)((double)dx * n / steps + (dx >= 0 ? 0.5 : -0.5));
        int y = y1 + (int)((double)dy * n / steps + (dy >= 0 ? 0.5 : -0.5));


        for (int a = -1; a <= 1; a+=1)
        {
            for (int b = -1; b <= 1; b+=1)
            {
                if (x + a >= 0 && x + a < picture.width &&
                    y + b >= 0 && y + b < picture.height)
                {
                    picture.at(x + a, y + b) = c;
                }
            }
        }
    }
}




void draw_map(Intersection * start, Connection * path[], int count)
{
    Intersection * destination = start;


    for (int n = count - 1; n >= 0; n-=1)
        destination = other_end(path[n], destination);
    DEM dem;


    if (!find_dem(start, destination, dem))
    {
        cout << "No DEM covers both places." << endl;
        return;
    }
    ifstream input(dem.filename.c_str(), ios::binary);
    if (!input)
    {
        cout << "Couldn't open " << dem.filename << endl;
        cout << "Extract the .dat files from all.zip first." << endl;
        return;
    }


    string word;
    int height, width, bytes_per_pixel, seconds_per_pixel;
    int left_seconds, top_seconds, minimum, maximum, special;


    input >> word >> height >> word >> width
          >> word >> bytes_per_pixel >> word >> seconds_per_pixel
          >> word >> left_seconds >> word >> top_seconds
          >> word >> minimum >> word >> maximum >> word >> special;
    if (!input || width <= 0 || height <= 0 || bytes_per_pixel != 2)
    {
        cout << "Invalid DEM dimensions." << endl;
        return;
    }


    input.clear();
    input.seekg(width * bytes_per_pixel, ios::beg);


    if (!input)
    {
        cout << "Invalid DEM header." << endl;
        return;
    }


    image picture(width, height);


    for (int y = 0; y < height; y+=1)
    {
        for (int x = 0; x < width; x+=1)
        {
            unsigned char bytes[2];
            if (!input.read((char *)bytes, 2))
            {
                cout << "Incomplete DEM data." << endl;
                return;
            }


            int elevation = (int)bytes[0] + 256 * (int)bytes[1];
            if (elevation >= 32768)
                elevation -= 65536;
            int c;
            if (elevation < 0)
                c = colour(60, 110, 170);
            else if (elevation < 300)
                c = colour(65, 145, 75);
            else if (elevation < 800)
                c = colour(135, 175, 85);
            else if (elevation < 1500)
                c = colour(195, 170, 110);
            else if (elevation < 2500)
                c = colour(145, 110, 85);
            else
                c = colour(235, 235, 225);
            picture.at(x, y) = c;
        }
    }


    Intersection * current = start;


    for (int n = count - 1; n >= 0; n-=1)
    {
        Intersection * next = other_end(path[n], current);
        draw_line(picture,
                  map_x(current->longitude, dem, picture.width),
                  map_y(current->latitude, dem, picture.height),
                  map_x(next->longitude, dem, picture.width),
                  map_y(next->latitude, dem, picture.height),
                  colour(255, 0, 255));
        current = next;
    }


    picture.write("map.bmp");
    cout << "Map saved to map.bmp using " << dem.filename << endl;
}


void shortest_route(Intersection * start, Intersection * destination)
{
    double distance[max_intersections];
    bool finished[max_intersections];
    Connection * previous[max_intersections];
    Connection * path[max_intersections];


    for (int n = 0; n < intersection_count; n+=1)
    {
        distance[n] = -1;
        finished[n] = false;
        previous[n] = NULL;
    }


    distance[start->id] = 0;


    while (true)
    {
        int closest = -1;
        for (int n = 0; n < intersection_count; n+=1)
        {
            if (finished[n] == false && distance[n] >= 0)
            {
                if (closest == -1 || distance[n] < distance[closest])
                {
                    closest = n;
                }
            }
        }
        if (closest == -1)
        {
            cout << "No road route connects these intersections." << endl;
            return;
        }
        finished[closest] = true;
        if (closest == destination->id)
        {
            break;
        }


        Intersection * current = &intersections[closest];
        RoadLink * temp = current->roads;


        while (temp != NULL)
        {
            Intersection * next = other_end(temp->road, current);
            double new_distance = distance[closest] + temp->road->length;
            if (finished[next->id] == false)
            {
                if (distance[next->id] < 0 || new_distance < distance[next->id])
                {
                    distance[next->id] = new_distance;
                    previous[next->id] = temp->road;
                }
            }


            temp = temp->next;
        }
    }


    int count = 0;
    Intersection * current = destination;


    while (current != start)
    {
        Connection * road = previous[current->id];
        path[count] = road;
        count+=1;
        current = other_end(road, current);
    }


    cout << endl << "Shortest route:" << endl;
    display_intersection(start);
    current = start;


    for (int n = count - 1; n >= 0; n-=1)
    {
        cout << "Take ";
        display_road(path[n], current);
        cout << endl << endl;
        current = other_end(path[n], current);
        display_intersection(current);
    }


    cout << fixed << setprecision(3);
    cout << "Total road distance: " << distance[destination->id] << " miles" << endl;
    draw_map(start, path, count);
}


int main()
{
    if (!read_states() || !read_places() || !read_graph())
    {
        cout << "Couldn't load the data files" << endl;
        return 1;
    }


    cout << "Welcome to the Road Map!" << endl;


    while (true)
    {
        cout << endl;
        cout << "Press 1 to explore roads" << endl;
        cout << "Press 2 to find a shortest route" << endl;
        cout << "Press 0 to quit" << endl;
        cout << "Choice: ";


        int choice = read_choice(2);


        if (choice == 0)
        {
            break;
        }


        cout << "Starting place:" << endl;
        Intersection * start = choose_intersection();


        if (start == NULL)
        {
            continue;
        }
        if (choice == 1)
        {
            explore(start);
        }
        else
        {
            cout << "Destination place:" << endl;
            Intersection * destination = choose_intersection();


            if (destination != NULL)
            {
                shortest_route(start, destination);
            }
        }
    }


    cout << "Goodbye!" << endl;
    return 0;
}