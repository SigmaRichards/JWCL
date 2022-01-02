library(stringi)
write.table(data.frame(n=stri_rand_strings(20000,4)),"rnames1.csv",row.names = FALSE,col.names = FALSE)
write.table(data.frame(n=stri_rand_strings(20000,4)),"rnames2.csv",row.names = FALSE,col.names = FALSE)