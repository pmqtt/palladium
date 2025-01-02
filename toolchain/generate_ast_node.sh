#!/bin/bash

# Check if a class name is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <ClassName>"
  exit 1
fi

CLASS_NAME=$1
HEADER_FILE="include/${CLASS_NAME}.h"
CPP_FILE="src/${CLASS_NAME}.cpp"

# Generate header file
cat <<EOL >$HEADER_FILE
#ifndef PALLADIUM_${CLASS_NAME^^}_H
#define PALLADIUM_${CLASS_NAME^^}_H

#include <memory>
#include <string>
#include "AstNode.h"

class $CLASS_NAME : public AstNode {
public:
  ~$CLASS_NAME() = default;
  $CLASS_NAME();
  void accept(const std::shared_ptr<Visitor>& v) override;
};

#endif // ${CLASS_NAME^^}_H
EOL

echo "$HEADER_FILE created."

# Generate C++ file
cat <<EOL >$CPP_FILE
#include "$CLASS_NAME.h"

$CLASS_NAME::$CLASS_NAME()
{
    // Constructor implementation
}

void $CLASS_NAME::accept(const std::shared_ptr<Visitor>& v) {
    // Implementation of accept method
}
EOL

echo "$CPP_FILE created."
