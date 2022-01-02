library(stringdist)

names <- as.character(read.csv("rnames1.csv",header = FALSE)[,1])
names2 <- as.character(read.csv("rnames2.csv",header = FALSE)[,1])

b <- Sys.time()
sd <- 1-stringdistmatrix(names,names2,method="jw",p=0.1)
e <- Sys.time()
ip <- length(sd)/as.numeric((e-b))

sum(sd>0.92)
sum(sd>0.88)-sum(sd>0.92)


ref = read.csv("../opencl/a.csv")
