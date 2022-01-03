library(stringi)
write.table(data.frame(n=stri_rand_strings(1000000,12)),"rnames1.csv",row.names = FALSE,col.names = FALSE)
write.table(data.frame(n=stri_rand_strings(100000,12)),"rnames2.csv",row.names = FALSE,col.names = FALSE)
