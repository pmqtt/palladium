#/bin/bash

echo "CMakeLists.txt" >gpt_critic.txt
echo "------------------------------" >>gpt_critic.txt
cat CMakeLists.txt >>gpt_critic.txt
echo "------------------------------" >>gpt_critic.txt

echo "src/CMakeLists.txt" >>gpt_critic.txt
echo "------------------------------" >>gpt_critic.txt
cat src/CMakeLists.txt >>gpt_critic.txt
echo "------------------------------" >>gpt_critic.txt

echo "tests/CMakeLists.txt" >>gpt_critic.txt
echo "------------------------------" >>gpt_critic.txt
cat tests/CMakeLists.txt >>gpt_critic.txt
echo "------------------------------" >>gpt_critic.txt

for f in src/*.cpp; do
  echo "$f" >>gpt_critic.txt
  echo "------------------------------" >>gpt_critic.txt
  cat $f >>gpt_critic.txt
  echo "------------------------------" >>gpt_critic.txt
done

for f in include/*.h; do
  echo "$f" >>gpt_critic.txt
  echo "------------------------------" >>gpt_critic.txt
  cat $f >>gpt_critic.txt
  echo "------------------------------" >>gpt_critic.txt
done

for f in tests/*.cpp; do
  echo "$f" >>gpt_critic.txt
  echo "------------------------------" >>gpt_critic.txt
  cat $f >>gpt_critic.txt
  echo "------------------------------" >>gpt_critic.txt
done
