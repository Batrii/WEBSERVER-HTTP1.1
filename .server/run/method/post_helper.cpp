#include ".server/run/method/post.cpp"


class urlencoder {
  private:
    std::map<std::string, std::string> _fields;
  public:
    void parseBodyContent(std::string& content, ctr& currentServer);
};

void urlencoder::parseBodyContent(std::string& content, ctr& currentServer)
{
  std::size_t i = 0;
  while (i < content.length())
  {
    std::string key;
    std::string value;
    // split with &
    std::size_t pos = content.find('&', i);
    std::string pair;
    if (pos != std::string::npos)
    {
      pair = content.substr(i, pos - i);
      i = pos + 1;
    }
    else
    {
      pair = content.substr(i);
      i = content.length();
    }
    // split with =
    std::size_t pos_eq = pair.find('=');
    if (pos_eq != std::string::npos)
    {
      key = pair.substr(0, pos_eq);
      value = pair.substr(pos_eq + 1);
    }
    else
    {
      key = pair;
      value = "";
    }
    this->_fields[key] = value;
  }
  std::string filepath = currentServer.uploaddir() + "form_urlencoded.txt";
  std::ofstream outfile(filepath.c_str());
  for (std::map<std::string, std::string>::iterator it = this->_fields.begin(); it != this->_fields.end(); ++it) {
    outfile << "Field Name: " << it->first << ", Value: " << it->second << "\n";
  }
  outfile.close();
}

int handle_multipart(const std::string& content, request& req , ctr& currentServer) {
  std::string boundaryname = req.getHeaders().at("Content-Type");
  std::size_t boundary_pos = boundaryname.find("boundary=");
  if (boundary_pos != std::string::npos){
    std::string boundary = "--" + boundaryname.substr(boundary_pos + 9);
    std::string boundary_break = boundary + "--";
    std::size_t i = 0;
    while (i < content.length()) {
      std::size_t boundary_start = content.find(boundary, i);
      std::size_t boundary_break_pos = content.find(boundary_break, i);
      if (boundary_break_pos != std::string::npos) {
        break;
      }
      if (boundary_start != std::string::npos){
        std::size_t boundary_end = content.find(boundary, boundary_start + boundary.length());
        if (boundary_end != std::string::npos) {
          std::string part = content.substr(boundary_start + boundary.length() + 2, boundary_end - (boundary_start + boundary.length() + 2));
          if (part.find("Content-Disposition: form-data;") != std::string::npos)
          {
            std::size_t filename_pos = part.find("filename=");
            if (filename_pos != std::string::npos) {
              std::string filename = part.substr(filename_pos + 10, part.find("\r\n", filename_pos) - (filename_pos + 10) - 1);
              std::size_t data_start = part.find("\r\n\r\n");
              if (data_start != std::string::npos) {
                std::string filedata = part.substr(data_start + 4, part.length() - (data_start + 4) - 2);
                std::string filepath = currentServer.uploaddir() + filename;
                std::ofstream outfile(filepath.c_str());
                outfile << filedata;
                outfile.close();
              }
            }
            else {
              // Handle regular form field if necessary
              std::size_t name_pos = part.find("name=");
              if (name_pos != std::string::npos) {
                std::string name = part.substr(name_pos + 6, part.find("\r\n", name_pos) - (name_pos + 6) - 1);
                std::size_t data_start = part.find("\r\n\r\n");
                if (data_start != std::string::npos) {
                  std::string fielddata = part.substr(data_start + 4, part.length() - (data_start + 4) - 2);
                  std::string stored_data = "Field Name: " + name + ", Value: " + fielddata + "\n";
                  std::ofstream outfile((currentServer.uploaddir() + "form_fields.txt").c_str());
                  outfile << stored_data;
                  outfile.close();
                }
              }
            }
          }
        }
        i = boundary_end;
      }
      else {
        break;
      }
    }
  }
  else {
    return -1;
  }
  return 0;
}

void handle_json(const std::string& content , ctr& currentServer) {
  std::ofstream outfile( (currentServer.uploaddir() + "data.json").c_str());
  outfile << content;
  outfile.close();
}