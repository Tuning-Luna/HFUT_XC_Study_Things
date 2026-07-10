#include <iostream>
#include <string>
#include <netinet/in.h>
#include <unistd.h>

int main() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in addr{};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8080);
  addr.sin_addr.s_addr = INADDR_ANY;

  bind(server_fd, (sockaddr*)&addr, sizeof(addr));
  listen(server_fd, 3);

  std::cout << "http://localhost:8080\n";

  while (true) {
    int client = accept(server_fd, nullptr, nullptr);

    char buffer[3000] = { 0 };
    read(client, buffer, 3000);

    std::string request(buffer);

    // 简单解析 q 参数
    std::string q = "";
    auto pos = request.find("q=");
    if (pos != std::string::npos) {
      q = request.substr(pos + 2);
      auto end = q.find(" ");
      q = q.substr(0, end);
    }

    std::string response =
      "HTTP/1.1 200 OK\nContent-Type: text/html\n\n"
      "<h1>Result:</h1>" + q;

    send(client, response.c_str(), response.size(), 0);
    close(client);
  }
}