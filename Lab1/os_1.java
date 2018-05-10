/* Mehul Jain 1001229017
CSE 3320 Programming Assignment 1*/


import java.util.*;
import java.io.*;

import java.awt.*;
import java.text.*;




public class os_1
{	


	public static void sort(ArrayList<String>sorting, String receive)
	{	if(receive.equals("directory"))
		{
				System.out.println("Sorted Directories:");
		}
		else
			System.out.println("Sorted Files:");
		
		Collections.sort(sorting);
		
		for(int i=0;i<sorting.size();i++)
		{
			System.out.println("\t\t\t"+i+" "+sorting.get(i));
		}
	}

	public static String stringcomparisonfile(String file_input, ArrayList<String>file_name)
	{	
		boolean check=true;
		
		try
		{
			int n=Integer.parseInt(file_input);
		
			check=true;
		
			return file_name.get(n);
		}
		catch(NumberFormatException e)
		{	
			boolean check1=true;
			
			if(check1==true )
			{
				String compare=file_input.substring(0,file_input.length() - 2);
			
				for(int i=0;i<file_name.size();i++)
			
				{	boolean checkagain=compare.equals(file_name.get(i).substring(0,file_input.length() - 2));
			
					if(checkagain==true)
			
						{
							return file_name.get(i);
						}
				}
			}
			else
				{return file_input;}
		}

		return file_input;
	}

	public static String stringcomparisondir(String file_input, ArrayList<String>file_name)
	{
		boolean check=true;
		
		try
		{
			int n=Integer.parseInt(file_input);
		
			check=true;
		
			return file_name.get(n);
		}
		catch(NumberFormatException e)
		{	
			boolean check1=true;
			
			if(check1==true )
			{
				String compare=file_input.substring(0,file_input.length() - 2);
			
				for(int i=0;i<file_name.size();i++)
			
				{	boolean checkagain=compare.equals(file_name.get(i).substring(0,file_input.length() - 2));
			
					if(checkagain==true)
			
						{
							return file_name.get(i);
						}
				}
			}
			else
				{return file_input;}
		}

		return file_input;
	}


	

	public static void main(String[] args) throws IOException 
	{
		int a=0, limiter=0, file_size=0;
		
		String command, file_input;

		double length;
		
		System.out.println("Current Working Directory = " + System.getProperty("user.dir")); //display current working directory

		Scanner s=new Scanner(System.in);

		ArrayList<String> file_name= new ArrayList<String>();

		String path= System.getProperty("user.dir");

		ArrayList<String> dir_name=new ArrayList<String>();

		Date date = new Date();
		
		System.out.println("It is now: "+date); 
		
		File folder = new File(System.getProperty("user.dir"));
		
		File[] listOfFiles = folder.listFiles();

		System.out.println("Files:");

		SimpleDateFormat dateFormat = new SimpleDateFormat("MM/dd/yyyy HH:mm");

		
		for (File file : listOfFiles) 
		{
			if (file.isFile())
			{	
				String run= " Read? "+ file.canRead();
				String exec=" Executable? "+ file.canExecute();
				System.out.println("\t\t\t"+a+" "+file.getName()+"\t("+file.length()+" bytes) \t|"+run+" | \t|"+exec+ "|\t|Date Modified: "+ dateFormat.format(file.lastModified())+"|");
				file_name.add(file.getName());
				a++;
				
			}
			for(int i=0;i<file_name.size();i++)  //setting the constraints for file name and number of files
		        		{
		        			if(file_name.size()==1024 || file_name.get(i).length()==2048)
		        			{
		        				System.exit(0);
		        			}
		        		}
		}

		a=0;
		System.out.println("Directories:");
		
		for (File file : listOfFiles) 
		{	
		if (file.isDirectory()) 
			{	
		
				String run= "Read? "+ file.canRead();
		
				String exec="Executable? "+ file.canExecute();
		
				System.out.println("\t\t\t"+a+" "+file.getName()+"\t("+file.length()+" bytes) \t|"+run+" | \t|"+exec+ "|\t|Date Modified: "+ dateFormat.format(file.lastModified())+"|");
		
				dir_name.add(file.getName());
		
				a++;
			}
			for(int i=0;i<dir_name.size();i++)
		        		{
		        			if(dir_name.size()==1024 || dir_name.get(i).length()==2048)   //setting the constraints for directory name and number of directories
		        			{
		        				System.exit(0);
		        			}
		        		}
		}

		int x=1;

		while(x==1)
		{
			System.out.println("Operation: ");
			
			System.out.println("\t\t\t E Edit ");
			System.out.println("\t\t\t R Run ");
			System.out.println("\t\t\t C Change Directory ");
			System.out.println("\t\t\t S Sort Directory Listing ");
			System.out.println("\t\t\t Q Quit ");

			System.out.print(">");
		
			command=s.next();

			switch(command)
				{
			
					case "E":	
							System.out.print(">Type the file name or the number or put * if unsure about the file name:");  // the user can use either the number or an incomplete file name too.
						
							file_input=s.next();

							file_input=  stringcomparisonfile(file_input,file_name); 
					
							try 
								{
					      			Desktop desktop = null;
		
					      			if (Desktop.isDesktopSupported()) 
					      				{
					        				desktop = Desktop.getDesktop();
					      				}
		
									desktop.edit(new File(System.getProperty("user.dir")+"/"+file_input));  // open any kind of file to be edited on the default app on the computer
								} 
		
						 	catch (IOException ioe) 
						   		{
						      		ioe.printStackTrace();
						   		}
												
							break;

					case "R":
							System.out.print(">Type the executable to be run or the number or put * if unsure about the executable name :");  // the user can use either the number or an incomplete executable name too.
						
							file_input=s.next();

							file_input= stringcomparisonfile(file_input,file_name);

							try 
								{
					      			Desktop desktop = null;
					      			
					      			if (Desktop.isDesktopSupported()) 
					      				{
					        				desktop = Desktop.getDesktop();
					      				}
									desktop.open(new File(System.getProperty("user.dir")+"/"+file_input)); //open any kind of executable to be run on the default OS service.
								} 
						 	catch (IOException ioe) 
						   		{
						      		ioe.printStackTrace();
						   		}

							break;

					case "C": 
								System.out.print(">Enter the name/number/incomplete name of the directory to be opened: ");
								
								file_input=s.next();

								file_input= stringcomparisondir(file_input,dir_name);
								
								System.setProperty("user.dir",path+"/"+file_input );

								folder = new File(System.getProperty("user.dir"));	
						
								System.out.println("Files:");

								listOfFiles = folder.listFiles();

								for (File file : listOfFiles) 
								{
		    						if (file.isFile()) 
		    							{	
									    		//long length=getFileSize(System.getProperty("user.dir")+"/"+file.getName());
									        	System.out.println("\t\t\t"+a+" "+file.getName()+" ("+file.length()+" bytes)"); // displaying the files with their size
									        	
									        	//file_name.add(file.getName());
									        	
									        	for(int i=0;i<file_name.size();i++)  //setting the constraints for file name and number of files
									        		{
									        			if(file_name.size()==1024 || file_name.get(i).length()==2048)
									        			{
									        				System.exit(0);
									        			}
									        		}
									        		
									        	a++;
									    	}
										}

									a=0;

									System.out.println("Directories:");
									
									for (File file : listOfFiles) 
										{
									    	if (file.isDirectory()) 
									    		{
									        		System.out.println("\t\t\t"+a+" "+file.getName()+" ("+file.length()+" bytes)");  // displaying the directories with their size
									        		
									        		//dir_name.add(file.getName());	        		
									        		
									        		for(int i=0;i<dir_name.size();i++)
									        		{
									        			if(dir_name.size()==1024 || dir_name.get(i).length()==2048)   //setting the constraints for directory name and number of directories
									        			{
									        				System.exit(0);
									        			}
									        		}
									        		
									        		a++;
									    		}
										}
									break;

				case "S":
						System.out.println("Sort the Listings. Enter 1 for directory or 2 for files");
						int k=s.nextInt();
						if(k==1)
						{	
							String send="directory";
							sort(dir_name,send);

						}
						else if(k==2)
						{	
								String send="files";
								sort(file_name,send);
						}
						else
							{
								System.out.println("Wrong number. Terminating...");
								
								System.exit(0);
							}

						break;


				case "Q": 
							System.out.println("Terminating....."); //Logout of the shell.
							System.exit(0);
				}
			}

		}

}

