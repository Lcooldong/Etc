import os
import sys

directory_list = ['./Batch', './Input_Images', './Video', './Music', './Mask']


def init_directory():
    try:
        for _idx, _dir in enumerate(directory_list):
            if not os.path.exists(_dir):
                os.makedirs(_dir)
                print("Creating Directory => " + _dir)

                f = os.popen('attrib +h' + _dir + '/.keep')
                if os.path.exists(_dir + '/.keep'):
                    print('insert .keep file => ' + _dir)
                f.close()
                # f = open('.keep', 'w')
                f.close()
            else:
                print("Already exists Dir => " + _dir)
                if not os.path.exists(_dir + '/.keep'):
                    # f = open(_dir + '/.keep', 'w')
                    f = os.popen('attrib +h' + _dir + '/.keep')
                    if os.path.exists(_dir + '/.keep'):
                        print('insert .keep file => ' + _dir)
                    f.close()
    except OSError:
        print('Error: Fail to Create directories.')


if __name__ == '__main__':
    init_directory()
