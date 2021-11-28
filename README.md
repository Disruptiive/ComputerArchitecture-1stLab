## 1ο Εργαστήριο Αρχιτεκτονικής Υπολογιστών

1) Ελέγχοντας το αρχείο [starter_se.py](https://github.com/uart/gem5-mirror/blob/master/configs/example/arm/starter_se.py) παρατηρούμε ότι ο default τύπος cpu αν δεν δώσουμε όρισμα είναι ο _atomic_

```python
parser.add_argument("--cpu", type=str, choices=list(cpu_types.keys()),
                        default="atomic",
                        help="CPU model to use")
```

η συχνότητα συστήματος είναι 1GHz (δεν μπορούμε να την αλλάξουμε με κάποιο όρισμα μόνο αλλάζοντας το starter_se.py αρχείο).

```python
self.clk_domain = SrcClockDomain(clock="1GHz",
                                         voltage_domain=self.voltage_domain)
```

Επιπλέον μέσω ορισμάτων μπορούμε να αλλάξουμε τη συχνότητα του επεξεργαστή (default 4GHz), τον αριθμό πυρήνων(default 4 πυρήνες), τον τύπο και την ταχύτητα της μνήμης RAM (default DDR3 1600Hz) και το μέγεθος της μνήμης (default 2GB).

```python
parser.add_argument("--cpu-freq", type=str, default="4GHz")
parser.add_argument("--num-cores", type=int, default=1,
                        help="Number of CPU cores")
parser.add_argument("--mem-type", default="DDR3_1600_8x8",
                        choices=ObjectList.mem_list.get_names(),
                        help = "type of memory to use")
parser.add_argument("--mem-channels", type=int, default=2,
                        help = "number of memory channels")
parser.add_argument("--mem-size", action="store", type=str,
                        default="2GB",
                        help="Specify the physical memory size")
```

Τέλος βλέπουμε ότι έχουμε L1 και L2 cache 

```python
if self.cpu_cluster.memoryMode() == "timing":
    self.cpu_cluster.addL1()
    self.cpu_cluster.addL2(self.cpu_cluster.clk_domain)
```

2) Διαβάζοντας το stats.txt και την επεξήγηση κάθε στατιστικού βλέπουμε οτι:
    * sim_seconds: Ο συνολικός χρόνος της προσομοίωσης σε δευτερόλεπτα
    * sim_insts: Ο αριθμός των εντολών που εκτελέι η CPU
    * host_inst_rate: Δείκτης απόδοσης του συστήματος, υπολογίζεται ως εντολές/δευτερόλεπτα

3) Απο την εκφώνηση ξέρουμε ότι (miss penalty) L1 = 6 cycles (miss penalty), L2 = 50 cycles και 1 cycle cache hit/instruction execution. Επίσης απο το αρχείο stats.txt μετά το simulation βλέπουμε ότι:

```text
system.cpu_cluster.cpus.dcache.overallMisses::total          179 

system.cpu_cluster.cpus.icache.overallMisses::total          332

system.cpu_cluster.l2.overallMisses::total                   479 

simInsts                                                     5028
```

Επομένως ```IL1.miss_num = 332, DL1.miss_num = 179,L2.miss_num = 479, Total_Inst_num = 5028``` και χρησιμοποιώντας την εξίσωση της εκφώνησης ![Formula](https://user-images.githubusercontent.com/43075884/143724729-b66cb8b2-3c21-4102-a2a6-818282ddf972.png)  
προκύπτει ότι  
![New eq](https://user-images.githubusercontent.com/43075884/143724745-9f366519-f747-4359-9e0c-b892b0535343.png)  
και τελικά  ```CPI = 6.37```