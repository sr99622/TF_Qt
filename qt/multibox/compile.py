import os
base = 'tensorflow/examples/qt/multibox'
files = os.listdir(base)
for f in files:
    parts = f.split('.')
    if parts[-1] == 'h':
        command = 'moc ' + base + '/' + f + ' -o ' + base + '/' + "moc_" + parts[0] + '.cc'
        os.system(command)

command = 'bazel build --config opt ' + base + '/...'
os.system(command)
