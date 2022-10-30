cd ./post &&
odb -d mysql --generate-query --generate-schema post.h &&
sed -i '1i\#pragma once' ./post-odb.ixx &&
cd .. &&
cd ./image &&
odb -d mysql --generate-query --generate-schema image.h &&
sed -i '1i\#pragma once' ./image-odb.ixx &&
cd ..