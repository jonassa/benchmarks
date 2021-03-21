#!/usr/bin/env python

def main():
  fp = open("load_old.cfg")
  fp_new = open("load-smooth.cfg", "w+")
  load_list = list()
  for line in fp:
    items = line.strip().split(',')
    load_list.append(int(items[1]))

  for i in range(len(load_list) - 1):
    difference = load_list[i + 1] - load_list[i]
    difference_cut = difference / 6
    load = load_list[i]
    for j in range(6):
      fp_new.write("10,{0}\n".format(load))
      load += difference_cut
  for i in range(6):
    fp_new.write("10,{0}\n".format(load_list[len(load_list)-1]))

  fp.close()
  fp_new.close()

if __name__ == "__main__":
  main()
