/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

DROP TABLE IF EXISTS `post`;

CREATE TABLE `post` (
  `id` BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `text` TEXT NOT NULL,
  `date` TEXT NOT NULL)
 ENGINE=InnoDB;

