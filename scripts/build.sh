set -e -u
set -o pipefail

if [[ ${COVERAGE:-false} == true ]]; then
    export LDFLAGS="--coverage"
    export CXXFLAGS="--coverage"
fi

ulimit -c unlimited -S

RESULT=0
make -j${JOBS:-1} test BUILDTYPE=${BUILDTYPE:-Release} V=1  || RESULT=$?
ls -la
for i in $(find ./ -maxdepth 1 -name 'core*' -print);
  do gdb $(which ./build/${BUILDTYPE}/tests) $i -ex "thread apply all bt" -ex "set pagination 0" -batch;
done;
if [[ ${RESULT} != 0 ]]; then exit $RESULT; fi

set +e +u
