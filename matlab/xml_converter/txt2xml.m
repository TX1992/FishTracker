%%
%convert .txt to .xml
%data in the.txt file has such a form:
%000002.jpg 132 121
%000002.jpg 140 110
%%
clc;
clear;
%note that remember to change the following four values
imgpath='../data/';%the directory of the folder where images place
txtpath='annotation_pos_12.txt';%the directory of the positive .txt file
txtpath_neg='annotation_neg_12.txt';%the directory of the negative .txt file
xmlpath_new='../xml_converter/Annotations/';%the directory to store .xml file

radius = 15;

fidin=fopen(txtpath,'r');
lastname='begin';

while ~feof(fidin)
     tline=fgetl(fidin);
     str = regexp(tline,'\t','split');
     str1 = regexp(str{1},'/','split');
     filepath=str{1};
     imgpath1=[str1{2} '/' str1{3} '/' str1{4} '/' str1{5}];
     img=imread(imgpath1);
     [h,w,d]=size(img);
      imshow(img);
      rectangle('Position',[str2double(str{2})-radius,str2double(str{3})-radius,radius*2,radius*2],'LineWidth',3,'EdgeColor','g');
      pause(0.1);
      
        if strcmp(str{1},lastname)%if the two files have the same name, then directly add objects
            object_node=Createnode.createElement('object');
            pos_node.appendChild(object_node);

            node=Createnode.createElement('center');
            node.appendChild(Createnode.createTextNode(sprintf('%s,%s',num2str(str{2}),num2str(str{3}))));
            object_node.appendChild(node);
        else %if the file name  is not equal, then need to create a new xml file
           copyfile(imgpath1, 'JPEGImages');
           if exist('Createnode','var')
              str2 = regexp(lastname,'/','split');
              tempname=str2{5};
              tempname=strrep(tempname,'.jpg','.xml');
              xmlwrite([xmlpath_new str1{4} '/' tempname],Createnode);
           end
            
            
            Createnode=com.mathworks.xml.XMLUtils.createDocument('annotation');
            Root=Createnode.getDocumentElement;%root node
            node=Createnode.createElement('folder');
            node.appendChild(Createnode.createTextNode(sprintf('%s',str1{4})));
            Root.appendChild(node);
            node=Createnode.createElement('filename');
            node.appendChild(Createnode.createTextNode(sprintf('%s',str1{5})));
            Root.appendChild(node);
            
           size_node=Createnode.createElement('size');
           Root.appendChild(size_node);

          node=Createnode.createElement('width');
          node.appendChild(Createnode.createTextNode(sprintf('%s',num2str(w))));
          size_node.appendChild(node);

          node=Createnode.createElement('height');
          node.appendChild(Createnode.createTextNode(sprintf('%s',num2str(h))));
          size_node.appendChild(node);

          pos_node=Createnode.createElement('positive');
          Root.appendChild(pos_node);


          object_node=Createnode.createElement('object');
          pos_node.appendChild(object_node);         

         node=Createnode.createElement('center');
         node.appendChild(Createnode.createTextNode(sprintf('%s,%s',num2str(str{2}),num2str(str{3}))));
         object_node.appendChild(node);
       
       lastname=str{1};
        end
        %process the last row
        if feof(fidin)
            tempname=strrep(str1{5},'.jpg','.xml');
            xmlwrite([xmlpath_new str1{4} '/' tempname],Createnode);
        end
end
fclose(fidin);

clear Createnode;

fidin=fopen(txtpath_neg,'r');
lastname='begin';

while ~feof(fidin)
     tline=fgetl(fidin);
     str = regexp(tline,'\t','split');
     str1 = regexp(str{1},'/','split');
     filepath=str{1};
     imgpath1=[str1{2} '/' str1{3} '/' str1{4} '/' str1{5}];
     img=imread(imgpath1);
     [h,w,d]=size(img);
     imshow(img);
     rectangle('Position',[str2double(str{2})-radius,str2double(str{3})-radius,radius*2,radius*2],'LineWidth',3,'EdgeColor','r');
     pause(0.1);
      
     xml_pos=strrep(str1{5},'.jpg','.xml');
     have_pos = exist([xmlpath_new str1{4} '/' xml_pos]);
      
      
     if strcmp(str{1},lastname)%if the two files have the same name, then directly add objects
            object_node=Createnode.createElement('object');
            neg_node.appendChild(object_node);

            node=Createnode.createElement('center');
            node.appendChild(Createnode.createTextNode(sprintf('%s,%s',num2str(str{2}),num2str(str{3}))));
            object_node.appendChild(node);
     else %if the file name  is not equal, then need to create a new xml file
           copyfile(imgpath1, 'JPEGImages');
           if exist('Createnode','var')
              str2 = regexp(lastname,'/','split');
              tempname=str2{5};
              tempname=strrep(tempname,'.jpg','.xml');
              xmlwrite([xmlpath_new str1{4} '/' tempname],Createnode);
           end
            
            
           if (have_pos)
                Createnode = xmlread([xmlpath_new str1{4} '/' xml_pos]);
                Root=Createnode.getDocumentElement;%root node
           else
                Createnode=com.mathworks.xml.XMLUtils.createDocument('annotation');
                Root=Createnode.getDocumentElement;%root node
                node=Createnode.createElement('folder');
                node.appendChild(Createnode.createTextNode(sprintf('%s',str1{4})));
                Root.appendChild(node);
                node=Createnode.createElement('filename');
                node.appendChild(Createnode.createTextNode(sprintf('%s',str1{5})));
                Root.appendChild(node);

                size_node=Createnode.createElement('size');
                Root.appendChild(size_node);

                node=Createnode.createElement('width');
                node.appendChild(Createnode.createTextNode(sprintf('%s',num2str(w))));
                size_node.appendChild(node);

                node=Createnode.createElement('height');
                node.appendChild(Createnode.createTextNode(sprintf('%s',num2str(h))));
                size_node.appendChild(node);
           end
          
           neg_node=Createnode.createElement('negative');
           Root.appendChild(neg_node);


           object_node=Createnode.createElement('object');
           neg_node.appendChild(object_node);

           node=Createnode.createElement('center');
           node.appendChild(Createnode.createTextNode(sprintf('%s,%s',num2str(str{2}),num2str(str{3}))));
           object_node.appendChild(node);
       
           lastname=str{1};
     end
        %process the last row
     if feof(fidin)
           %tempname=lastname;
            tempname=strrep(str1{5},'.jpg','.xml');
            xmlwrite([xmlpath_new str1{4} '/' tempname],Createnode);
     end
end
fclose(fidin);