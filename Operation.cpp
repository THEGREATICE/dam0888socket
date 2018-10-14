#include "Operation.h"

#include <syslog.h>

Operation::Operation(const char name[], int port, int addr):ioport_(port), ioaddr_(addr), name_(name) {
  state_ = 0;
}

void Operation::execute() {

}

//////////////////////////////////////////////////////////////////////////
OperationDefine::OperationDefine() {
  keyFile_ = g_key_file_new();
}

OperationDefine::~OperationDefine() {
  g_key_file_free(keyFile_);
}

std::vector<Operation*> OperationDefine::create(const std::string &file) {
  GError *error = NULL;
  std::vector<Operation*> ops;
  if(!g_key_file_load_from_file(keyFile_, "devices.ini", G_KEY_FILE_NONE, &error)) {
    syslog(LOG_CRIT, "Device configure failed! (%s)", error->message);
    return ops; //TODO sth will do
  }
  gchar **groups = g_key_file_get_groups(keyFile_, NULL);
  while(groups != NULL) {
    int port = g_key_file_get_integer(keyFile_, *groups, "ioport", &error);
    int addr = g_key_file_get_integer(keyFile_, *groups, "ioaddr", &error);
    Operation *op = new Operation(*groups, port, addr);
    ops.push_back(op);
    groups += 1;
  }
  return ops;
}
