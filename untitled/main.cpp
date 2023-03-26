#include <iostream>
#include "tuple"
#include <cmath>
#include "random"


#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

std::tuple<int, int> GetCell(double pos_x, double pos_y, double resolution, int row, int col){
    int x_out = 0;
    int y_out = 0;
    if(std::abs(pos_x) > resolution)
        x_out = round(pos_x/resolution + col/4.5) -1;
    if (std::abs(pos_y) > resolution)
        y_out = round(pos_y/resolution + row/2.5) - 1;
    if(x_out < 0 || y_out < 0)
        return {0,0};
    return {x_out, y_out};

}



int main() {



    // Listen for incoming connections

    //srand(time(nullptr)); // set the seed based on the current time
    int size_x = 2000;
    int size_y = 2000;

    bool **test = new bool*[size_y];
    for (int row = 0; row < size_y; ++row) {
      test[row] = new bool[size_x];
    }

    double resolution = 0.1;
    //bool test[size_x][size_y];
    for (int row = 0; row < size_y; ++row) {
        for (int col = 0; col < size_x; ++col) {
            test[row][col] = false;
        }
    }
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PCDReader reader;
    reader.read<pcl::PointXYZ>("/home/nikolai/Downloads/scan.pcd", *cloud);

    for (size_t i = 0; i < cloud->size(); ++i)
    {
        auto x_pos = cloud->points[i].x - cloud->points[0].x;
        auto y_pos = cloud->points[i].y - cloud->points[0].y;
        auto cell_pos = GetCell(x_pos, y_pos, resolution, size_y, size_x);
        if (std::get<0>(cell_pos) < size_y && std::get<1>(cell_pos) < size_x) {
            test[std::get<1>(cell_pos)][std::get<0>(cell_pos)] = true;
        }
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }
    for (int row = 0; row < size_y; ++row) {
        for (int col = 0; col < size_x; ++col) {
                //std::cout<<row<<std::endl<<col<<std::endl;
        }
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost
    addr.sin_port = htons(12345); // port number

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cerr << "Failed to connect to server\n";
        close(sock);
        return 1;
    }

    // Accept connection
    int connfd = accept(sock, NULL, NULL);
    bool *buffer = new bool[size_x*size_y];
    for (int i = 0; i <size_y ; ++i) {
        memcpy(&buffer[i*size_y], test[i], size_y*sizeof(bool));
    }
    //bool buffer[size_x*size_y];
    send(sock, buffer,  size_x * size_y,   0);
    for (int i = 0; i < size_y; i++) {
        delete[] test[i]; // free memory for each row
    }
    delete[] test;        // free memory for the array of pointers
    delete[] buffer;
    return 0;
}
