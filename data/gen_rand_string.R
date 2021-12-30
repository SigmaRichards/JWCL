file_name = "rnames.csv"

rstring <- function(l = 12, n = 5000) {
  do.call(paste0,replicate(l,sample(c(letters,LETTERS,0:9), n, TRUE),FALSE))
}

write.table(data.frame(n=rstring(12,5000)),file_name,row.names = FALSE,col.names = FALSE)
