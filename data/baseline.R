library(stringdist)

names <- as.character(read.csv("rnames.csv",header = FALSE)[,1])
names2 <- names

b <- Sys.time()
sd <- 1-stringdistmatrix(names,names2,method="jw")
e <- Sys.time()
ip <- length(sd)/as.numeric((e-b))

dd = as.numeric(sd)

df = data.frame(s=dd)
write.table(df,"rjw.csv",row.names = FALSE,col.names = FALSE)
